

thread_stack dd 0x200000
act_process dd 0x0

max_thread dd 0x0
act_thread dd 0x0
proc_block dd 0x0

construct_ldt_descriptor:	; Constructs a LDT descriptor and sets it active
				; EAX - LDT address
				; ECX - LDT size

	mov [ds:ldt_desc], cx
	mov [ds:ldt_desc+2], ax
	mov ebx, eax
	and ebx, 0x00FF0000
	shr ebx, 16
	mov bh, 10000010b
	mov [ds:ldt_desc+4], bx
	and eax, 0xFF000000
	shr eax, 8
	mov [ds:ldt_desc+6], ax
	mov ax, 11000b
	lldt ax
	ret

debug_thread_now:				; Temporary procedure. List the now process/thread
	mov eax, [ds:act_thread]
	call ConvertEAXtoEBXECX
	mov [ds:_thrd], ebx
	mov [ds:_thrd+4], ecx

	mov eax, [ds:act_process]
	call ConvertEAXtoEBXECX
	mov [ds:_proc], ebx
	mov [ds:_proc+4], ecx

	mov [ds:VCCurrentCurpos], dword 0

	mov eax, [ds:0x12000]
	call ConvertEAXtoEBXECX
	mov [ds:_msgs], ebx
	mov [ds:_msgs+4], ecx

	
	lea ecx, [thne]
	mov bh, 0x1F
	call CONSOLE_TextType
	ret
	
	thne db 'Process: '
	_proc db '         Thread: '
	_thrd db '          Messages: ',
	_msgs db '        ',0
	

store_task:				
		; INTERRUPT ENTRY POINT HERE

	push ds
	push eax
	mov ax, 10h
	mov ds, ax
	pop eax
	xchg [ds:thread_stack], esp		; Stack to PSS
	push gs
	push fs
	push es
	xchg [ds:thread_stack], esp		; Stack to Kernel
	push eax
	mov ax, 10h
	mov gs, ax
	pop eax
	pop ds
	xchg [gs:thread_stack], esp		; Stack to PSS
	push ds
	push eax
	push ebx
	push ecx
	push edx
	push esi
	push edi
	push ebp
	xchg [gs:thread_stack], esp		; Stack to Kernel
	pop eax
	pop ebx
	pop ecx
	pop edx
	pop esi
	xchg [gs:thread_stack], esp		; Stack to PSS
	push esi
	push edx
	push ecx
	push ebx
	push eax
	xchg [gs:thread_stack], esp		; Stack to PSS


	mov ax, 10h
	mov ds, ax
	mov es, ax
	
scheduler:

	mov edx, [ds:proc_block]		; Get current Process Block Address
	mov eax, [ds:edx]			; Get Max Threads
	mov [ds:max_thread], eax		; Update current Max Threads
	mov ebx, [ds:act_thread]		; Get the now thread

	cmp eax, 1				; If there is only 1 thread...
	jnz scheduler_thread_non1		; ... if not then jump

	cmp ebx, 1				; If we would execute same thread again..
	jz scheduler_select_process		; ... then pick another process

	mov [ds:act_thread], dword 1		; Set first thread as now

	mov eax, 0x80				; Set Process Saver Stack's offset
	add eax, edx				; Add Process Block Address to this
	jmp near load_task			; Load the thread

scheduler_thread_non1:

	cmp eax, ebx				; Check if it is the final thread...
	jz scheduler_select_process		; ... if it is, pick another process
	inc ebx					; Increase process number
	mov [ds:act_thread], ebx  		; Save it
	mov eax, [ds:proc_block]		; Get current Process Block Address
	inc ebx						; It's gonna be top address, not bottom
	shl ebx, 6					; Multiply if by Process Saver Stack's size
	add eax, ebx				; Add offset to the Process Block Address
	jmp near load_task			; Load the thread
	

scheduler_select_process:
	
	mov eax, 0x01				; Set the Process List identifier
	call MuiMan_GetFileDataPointer		; Get it's base

	mov eax, [ds:ebx]			; Get process's number
	mov edx, ebx

	cmp eax, [ds:act_process]		; If now process is the last...
	jnz scheduler_select_process_notlast	; ... if not, then jump
	mov [ds:act_process], dword 1		; Set first process
	jmp scheduler_load_process		; Load it
	

scheduler_select_process_notlast:

	inc dword [ds:act_process]		; Set next process
	jmp scheduler_load_process		; Load it!
		

scheduler_load_process:

	mov [ds:act_thread], dword 0			; Nullify thread
	
	mov eax, [ds:act_process]		; Get current process
	dec eax					; For calculations \/
	shl eax, 4				; Multiply by entry size

	add eax, 4				; Skip the DWORD with process number
	
	add eax, edx				; Add the Process List's base to the offset

	mov ecx, [ds:eax]			; ECX is Process Entry's first DWORD, so it's Process Information Block addr

	mov [ds:proc_block], ecx

	mov eax, [ds:ecx]			; Take thread's number
	mov [ds:max_thread], eax		; Save it

	mov eax, [ds:ecx+4]			; Get LDT address
	mov ecx, [ds:ecx+8]			; Get LDT offset
	call construct_ldt_descriptor		; Create the LDT

	jmp near scheduler			; Go to thread picking
	


load_task:
			; EAX points to high address

	push eax
	call debug_thread_now
	pop eax

	mov [ds:thread_stack], eax
	mov esp, eax
	sub esp, 44
	pop ebp	
	pop edi	
	pop esi	
	pop edx	
	pop ecx	
	pop ebx	
	pop eax	
	pop ds	
	pop es	
	pop fs	
	pop gs	
	sub esp, 64
	iret