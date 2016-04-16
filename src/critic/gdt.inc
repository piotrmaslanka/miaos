;Mia Exokernel - /critic/gdt.inc
;Copyright (C) Mia Exokernel Project Group

;This program is free software; you can redistribute it and/or
;modify it under the terms of the GNU General Public License
;as published by the Free Software Foundation; either version 2
;of the License, or (at your option) any later version.

	; Critical MIA OS GDT Layout

gdt:                  

gdt_null:
        dd 0
        dd 0

gdt_sys_code: 
        dw 0FFFFh
        dw 0
        db 0
        db 10011010b
        db 11001111b
        db 0

gdt_sys_data:            
        dw 0FFFFh
        dw 0
        db 0
        db 10010010b
        db 11001111b
        db 0

ldt_desc: 
	dw LDTSize
	dw ldt
	db 0
	db 10000010b
	db 0
	db 0
tss_desc:
	dw 103
	dw tss
	db 0
	db 11101001b
	db 0
	db 0
backup:
	dd 0
	dd 0
backup2: 
	dd 0
	dd 0

gdt_end: 



gdt_desc:                       
        dw gdt_end - gdt - 1    
        dd gdt    

ldt:
        dw 0FFFFh
        dw 0
        db 0
        db 11111010b
        db 11001111b
        db 0

        dw 0FFFFh
        dw 0
        db 0
        db 11110010b
        db 11001111b
        db 0
ldt_end:

	LDTSize equ ldt_end - ldt - 1