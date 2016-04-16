
SendMessage:
				; ECX = pointer to message	EDX = sender
	push ecx
	push edx
	mov eax, 2						; Parameter = Message List identifier
	call MuiMan_GetFileDataPointer				; Retrieve the Message List
	jc fx_error
	pop edx
	pop ecx
	push ebx						; Store original pointer
	
	mov esi, [ds:ebx]					; Get message count
cont:

	mov eax, 2						; Parameter = Message List identifier
	mov ebx, ecx						; Parameter = Pointer to message
	shl esi, 5						; Multiply count by 32 to get new msg place
	add esi, 4
	mov edx, esi						; Parameter = Destination
	mov ecx, 32						; Parameter = Count, message size
	mov ebp, [ds:act_process]
	mov [es:ecx], ebp					; Set sender identity

	call MuiMan_WriteToFile					; Write it!\\		

	jnc SendMessage_End					; If not failed, jump
fx_error:
	mov eax, 0x12345678					; Else set the flag
	ret							; Return
SendMessage_End:				
	xor eax, eax						; Clear failure sign
	pop ebx							; Restore original pointer
	inc dword [ds:ebx]					; Increase message count
	ret							; Return

CheckMessage:	;EBP = buffer
	
	mov esi, [ds:act_process]
	push ebp				; Stos +1				
	mov eax, 2
	call MuiMan_GetFileDataPointer
	pop ebp					; Stos 0
	push ebx				; Stos +1
	jc stcr
	mov eax, ebx
	mov ecx, [ds:ebx]					; Set repeat counts to message count
	cmp ecx, 0
	jz stcr
	add ebx, 4						; Skip message count
rchkmsg:
	mov edx, [ds:ebx+4]					; Get recipient
	cmp edx, esi						; If recipient the same as message recipient...
	jz msg_found						; ... then jump
	add ebx, 32						; Go to next message
	loop rchkmsg						; Decrease messages to view and jump
stcr:
	pop eax
	mov eax, 0x12345678					; Set carry flag	(failed)
	ret							; Return
	
msg_found:	
			; EBX - adres wiadomosci

	push ebx
	
	mov esi, ebx						; Ustaw wiadomo��
	mov edi, ebp						; Ustaw bufor
	mov ecx, 8						; Ustaw rozmiar 8*d = 32
	rep movsd						; Kopiuj

	pop ebp
	pop ebx					; Stos 0
	push es					; Stos +1
	push 10h				; Stos +2
	pop es					; Stos +1
	
	push eax				
	mov ecx, [dword ds:eax]			; ECX = now message count
	dec ecx					; ECX = now message
	shl ecx, 5				; Multiply it by 32
	add eax, ecx				; Add it to Message Address 
	add eax, 0x4				; Add message count overridding (complete now)
	mov esi, eax				; Set it as start
	mov edi, ebp				; Use message address as buffer
	mov ecx, 8
	rep movsd
	pop eax					; Stack +4
	dec dword [ds:eax]
	xor eax, eax
	pop es					; Stack 0
	ret

	

	