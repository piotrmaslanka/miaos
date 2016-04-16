
keyb_write_to_buffer:
		; AL's the key
	mov bx, word [ds:KeybBufferSize]
	cmp bx, KeybBufferMaxSize
	jz keyb_write_to_buffer_end
	mov [ds:KeybBuffer+bx], al
	inc word [ds:KeybBufferSize]		
keyb_write_to_buffer_end:
	ret


keyb_read_buffer:
		; AL's the key
	xor al, al
	mov bx, word [ds:KeybBufferSize]
	cmp bx, 0
	jz keyb_read_buffer_end
keyb_read_buffer_word:
	dec word [ds:KeybBufferSize]
	dec bx
	mov al, [ds:KeybBuffer+bx]
keyb_read_buffer_end
	ret

keyboard_table_normal:
				; 39 is '
	db 0,27,'1234567890-=',8,9,'qwertyuiop[]',10,0,'asdfghjkl;',39
	db 0,'\zxcvbnm,./',0,0,0,' '
	times 24 db 0



keyboard_edata:
	db 01
	times 25 db 0
	db 06
	times 11 db 0
	db 33
	times 11 db 0
	db 34
	db 36
	db 08
	db 0
	db 35
	db 10,11,12,13,14,15,16,17,18,19
	db 37,38,27,28,29,30,24,25,26,31,21,22,23,20,32

keyboard_state:	db 0
	; Bit 0	-	CAPSLOCK	-	0x58
	; Bit 1 -	LALT		-	0x11
	; Bit 2 -	RALT		- 0xE0 0x14	
	; Bit 3 -	LSHIFT		-	0x12
	; Bit 4 -	RSHIFT		-	0x59
	; Bit 5 -	LCTRL		-	
	; Bit 6 -	RCTRL		-	
	

keyboard_driver_depressed:
	jmp keyboard_driver_exit_point

keyboard_driver_entry_point:

	xor ax, ax
	in al, 0x60

	cmp al, 0
	jz keyboard_driver_exit_point


	test al, 128
	jnz keyboard_driver_depressed

	mov bp, ax
	mov al, [ds:keyboard_table_normal+bp]
	
	call keyb_write_to_buffer	

keyboard_driver_exit_point:
	in  al, 61h
	mov ah, al	
	or  al, 80h	
	out 61h, al
	mov al, ah
	out 61h, al
	ret
	