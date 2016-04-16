
SysStop:
	mov ax, 0xff
	mov ds, ax
	cli
	hlt


SysCallDispatcher:

	cli

	push ds				; Save DS for application
	push 10h			; Push system segment
	pop ds				; Save it to DS

	mov [ds:0xb8200], word 0x1234

	cmp eax, 0			; 
	jnz n1

n1:
	cmp eax, 1
	jnz n2
	mov ebx, [ds:act_thread]
	mov ecx, [ds:act_process]
	mov edx, [ds:max_thread]
	jmp rets

n2:
	cmp eax, 2
	jnz n3
	call SendMessage
	jmp rets

n3:
	cmp eax, 3
	jnz rets
	call CheckMessage
	jmp rets
	
	mov [ds:0xb8200], word 0x0000


rets:
	pop ds
	sti
	iret

;		SysCall 1  -  GetProcessInformation(EBX=thread,ECX=process,EDX=maxthread)
;		SysCall 2  -  SendMessage(ECX=pointer)
