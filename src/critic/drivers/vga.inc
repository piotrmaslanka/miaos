

etwas dd 160

_repeat:

	mov ax, 1111b
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
_joop:
		
	mov eax, 3
	lea ebp, [message]
	mov esi, 2
	int 80h
	cmp eax, 0x12345678
	jz _joop

	mov ebx, [ds:etwas]
	add dword [ds:etwas], 8
	mov [ds:0xb8000+ebx], dword 0x56789ABC
	jmp _joop

__repeat:

	mov ax, 1111b
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
__joop:
		
	mov eax, 3
	lea ebp, [message]
	mov esi, 3
	int 80h
	cmp eax, 0x12345678
	jz __joop

	mov ebx, [ds:etwas]
	add dword [ds:etwas], 8
	mov [ds:0xb8000+ebx], dword 0xABCDEF01
	jmp __joop

_message:
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0