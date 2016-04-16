
;	DS system ES desired segment


MuiMan_GetFilePointer:	
; MuiMan_GetFilePointer(EAX Handle) EBX
	mov ebx, MUIMANStart
flex1:
	mov ecx, [ds:ebx]
	cmp ecx, 0
	jnz constr_notszero
	stc
	ret	
constr_notszero:
	mov edx, ebx
	and edx, 00000000000011111111111111111111b 
	cmp edx, 4080
	jnz constr_notsend
	mov ebx, ecx
	jmp flex1
constr_notsend
	and ecx, 00000000000011111111111111111111b
	cmp ecx, eax
	jnz constr_notsthat
	clc
	ret
constr_notsthat:
	add ebx, 0x10
	jmp flex1

MuiMan_DetermineAccessRights:
;MuiMan_DetermineAccessRights(EAX Handle, EDX Identifier, ECX Privilenges+31:Purpose(SET=read)) CarryFlag (SET=no access)
	
	call MuiMan_GetFilePointer
	jnc MuiMan_DAR_nof1
	stc
	ret
MuiMan_DAR_nof1:
	cmp edx, dword [ds:ebx+4]			; is it the owner..
	jz MuiMan_DAR_succeed			; ... if yes, grant access

	test edx, dword [ds:ebx]
MuiMan_DAR_succeed:
	clc
	ret
	

		



MuiMan_GetMuiHeader:
; MuiMan_GetMuiHeader(EAX Handle) EBX+ECX+EDX+EDI

	push eax
	call MuiMan_GetFilePointer
	pop eax
	jnc emMC002
	jmp near MuiShared_Fault
emMC002:
	mov ebp, ebx
	mov ebx, [ds:ebp]		
	mov ecx, [ds:ebp+4]
	mov edx, [ds:ebp+8]
	mov edi, [ds:ebp+12]
	clc
	ret



MuiMan_GetFileDataPointer:
; MuiMan_GetFileDataPointer(EAX Handle) EBX
	
	call MuiMan_GetMuiHeader
	jnc ertertce
	jmp near MuiShared_Fault
ertertce:
	test edx, 0x80000000
	jz notswapped
	push eax
	call MuiMan_SwapIn
	pop eax
	push eax
	call MuiMan_GetFilePointer
	pop eax
	mov edx, [ds:ebx+8]
	or edx, 0x80000000
	mov [ds:ebx+8], edx
	jmp MuiMan_GetFileDataPointer
notswapped:
	and edx, 00000000000011111111111111111111b
	shl edx, 12
	mov ebx, edx
	clc
	ret



MuiMan_WriteToFile:
; MuiMan_WriteToFile(EAX Handle, EBX Buffer, EDX Position, ECX Count);
	push eax
	push ebx
	push ecx
	push edx
	push edx
	push ecx
	call MuiMan_GetMuiHeader
	jnc neps
	jmp near MuiShared_Fault	
neps:	pop ecx
	pop edx				;EDI by� rozmiar pliku
	add edx, ecx
	cmp edx, edi
	jna emMC001
	pop ecx
	pop ecx
	pop ecx
	pop ecx
	jmp near MuiShared_Fault
emMC001:
	call MuiMan_GetFileDataPointer
	mov edi, ebx
	pop edx
	pop ecx
	pop ebx
	pop eax		; 0x4da
	add edi, edx
	push ds
	push es
	pop ds
	pop es
	mov esi, ebx
	rep movsb
	push ds
	push es
	pop ds
	pop es
	clc
	ret	

MuiMan_ReadToFile:
; MuiMan_ReadFile(EAX Handle, EBX Buffer, EDX Position, ECX Count);
	push eax
	push ebx
	push ecx
	push edx
	push edx
	push ecx
	call MuiMan_GetMuiHeader
	jc MuiShared_Fault
	pop ecx
	pop edx				;EDI by� rozmiar pliku
	add edx, ecx
	cmp edx, edi
	jna emMC003
	pop ecx
	pop ecx
	pop ecx
	pop ecx
	jmp near MuiShared_Fault
emMC003:
	call MuiMan_GetFileDataPointer

	mov esi, ebx
	pop edx
	pop ecx
	pop ebx
	pop eax		

	add esi, edx
	mov edi, ebx

	rep movsb
	clc
	ret	


MuiMan_SwapIn:
			; For now void, but has to be
	cli	
	hlt
	ret

MuiMan_AddFolderSpace:
	mov ebx, MUIMANStart
lepperex:
	mov ecx, [ds:ebx]
	mov edx, ebx
	and edx, 111111111111b
	cmp edx, 4080
	jnz retry_it
	cmp ecx, 0
	jnz patch_next_dir
	push ebx
	call memman_allocate_single_block
	pop ebx
	mov [ds:ebx], eax			; Install new directory!
	mov edi, eax
	mov ecx, 0x1000
	call clearmemory
	clc
	ret
patch_next_dir:
	mov ebx, ecx
	jmp lepperex
retry_it:
	cmp ecx, 0
	jnz nemptfile
	jmp MuiShared_Fault
nemptfile:
	add ebx, 0xF
	jmp lepperex	

MuiShared_Fault:
	stc
	ret

	
	


