
; Console Models

;	DS	-	System Segment
;	ES	-	Desired Read Segment

;	DO NOT EXPECT REGSAVING
;	THEY ARE NEARCALLS MODELS

CONSOLE_TextType:
	; ECX is Text Address finished with '0'
	; BH is Text Attribute

	xor ax, ax
	xor bl, bl

 console_texttype_writeagain:
	
	mov bl, [ES:ECX]
	cmp bl, 0
	jnz console_texttype_nozero
	ret
 console_texttype_nozero:
	push ebx
	push ecx
	call CONSOLE_CharType
	pop ecx
	pop ebx
	inc ecx
	jmp console_texttype_writeagain
	

CONSOLE_CharType:
	; BX is Char with its Attribute

	mov ax, word [DS:VCCurrentCurpos]
	inc ax
	mov [DS:VCCurrentCurpos], ax
	
	dec ax
	mov cx, 2
	mul cx
	mov bp, ax
	and ebp, 65535

	mov word [DS:GRAFTEXTSTART+ebp], bx
	ret
