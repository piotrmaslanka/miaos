;Mia Exokernel - /critic/devices.inc
;Copyright (C) Mia Exokernel Project Group

;This program is free software; you can redistribute it and/or
;modify it under the terms of the GNU General Public License
;as published by the Free Software Foundation; either version 2
;of the License, or (at your option) any later version.

; Here is the list of compiled devices
;  and the list of IDT pointers to serve them



DevKeyboardIDT:

dw DevKeyboardPtr 
dw SystemCodeSeg
dw 0x8E00
dw 0

DevTimerIDT:
dw DevTimerPtr
dw SystemCodeSeg
dw 1110111000000000b
dw 0

GPF_IDT:
dw DevGPFPtr
dw SystemCodeSeg
dw 0x8E00
dw 0

TSS_IDT:
dw DevTSSPtr
dw SystemCodeSeg
dw 0x8E00
dw 0

IOP_IDT:
dw DevIOPPtr
dw SystemCodeSeg
dw 0x8E00
dw 0

Interruptor_IDT:
dw store_task
dw SystemCodeSeg
dw 1110111000000000b
dw 0

SysCallDispatcher_IDT:
dw SysCallDispatcher
dw SystemCodeSeg
dw 1110111000000000b
dw 0

convertable: db '0123456789ABCDEF'

ConvertALtoBX:
	push eax
	and eax, 00000000000000000000000000001111b

	mov bl, [ds:convertable+eax]	

	pop eax
	and eax, 00000000000000000000000011110000b
	shr eax, 4
	
	mov bh, [ds:convertable+eax]
	xchg bh, bl
	ret


ConvertAXtoEBX:
	xor ebx, ebx
	xor ecx, ecx
	push eax

	call ConvertALtoBX
				
	pop eax				
	mov cx, bx			

	shr ax, 8
	call ConvertALtoBX		
	shl ecx, 16			

	add ebx, ecx
	ret

ConvertEAXtoEBXECX:			; Wy¿sza szko³a jazdy =]

	push eax

	call ConvertAXtoEBX

	mov ecx, ebx
	pop eax
	push ecx
	shr eax, 16
	call ConvertAXtoEBX
	pop ecx
	ret	

DevIOPPtr:
	mov ebp, '#OPC'
	jmp near DevErrPtr

DevGPFPtr:
	mov ebp, '#GPF'
	jmp near DevErrPtr

DevTSSPtr:
	mov ebp, '#TSS'
	jmp near DevErrPtr

DevErrPtr:

	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov [ds:VCCurrentCurpos], dword 80

	mov esp, 0x8000
	mov eax, ebp
	call ConvertEAXtoEBXECX
	mov [ds:_exception], ebx
	mov [ds:_exception+4], ecx

	pop eax
	pop ebx
	pop ecx

	push ebx
	push ecx

	call ConvertEAXtoEBXECX
	
	mov [ds:_offset], ebx
	mov [ds:_offset+4], ecx

	pop eax

	call ConvertEAXtoEBXECX
	
	mov [ds:_segment], ecx

	pop eax

	call ConvertEAXtoEBXECX

	mov [ds:_errcode], ebx
	mov [ds:_errcode], ecx

	lea ecx, [error]
	mov bh, 0x1F
	call CONSOLE_TextType
jopka:	jmp jopka
	error db 'Exception: 0x'
	_exception db '        '
	_dalej db '. Error happened at:  '
	_segment db '    :'
	_offset db '        . Errorcode: '
	_errcode db '        ','          '

	_newline db 'An exception happened and system was forced to stop.',0	

DevTimerPtr:
	push eax
	mov al, 0x20
	out 0x20, al	
	pop eax
	jmp store_task


	

DevKeyboardPtr:
	pusha
	inc dword [ds:KbdInt]
	mov al, 0x20
	out 0x20, al
	popa
	iret

%include "critic\drivers\keyboard.inc"