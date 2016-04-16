
	SECT_KERNEL_SIZE EQU 0x8
	LOAD_KERNEL_ADDR EQU 0x0
	BYTES_PER_SECTOR EQU 0x200

		[ORG 0x7C00]
		[BITS 16]

	xor ax, ax
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov sp, 0x7F00			; Setup Stack

	int 13h				; Reset disk

	
	
	mov ah, 02h
	mov al, SECT_KERNEL_SIZE
	mov cl, 02h
	xor ch, ch
	xor dh, dh
	mov bx, 0x600
	int 13h				; Read kernel

      

	call    empty_8042
        mov     al,0xd1               
        out     0x64,al
        call    empty_8042
        mov     al,0xdf
        out     0x60,al
        call    empty_8042
	jmp aftera20

empty_8042:
        in      al,0x64
        test    al,2
        jnz     empty_8042
        ret

aftera20:

	cli
	lgdt [gdt_desc]

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	jmp dword 08h:runos

		[BITS 32]

runos:
	mov ax, 10h
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov esp, 0xc0000000

	mov esi, 0x600
	mov edi, LOAD_KERNEL_ADDR
	mov ecx, SECT_KERNEL_SIZE * BYTES_PER_SECTOR

	rep movsb

	jmp 08h:LOAD_KERNEL_ADDR

gdt:                  

gdt_null:
        dd 0
        dd 0

gdt_code: 
        dw 0FFFFh
        dw 0
        db 0
        db 10011010b
        db 11001111b
        db 0

gdt_data:            
        dw 0FFFFh
        dw 0
        db 0
        db 10010010b
        db 11001111b
        db 0

gdt_end: 



gdt_desc:                       
        dw gdt_end - gdt - 1    
        dd gdt                  




	times 510-($-$$) db 0      

        dw 0AA55h               
	
