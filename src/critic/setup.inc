
;
;		Prepare initprocs to work
;	
				; Set up GDT
	lgdt [gdt_desc]
	jmp 0x08:clear_cache
clear_cache:
				; Set up the registers
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	lidt [myidt]
				; Set up the stack
	mov esp, StackStart
		
				; Floppy Motor - [OFF]
	mov dx, 0x3F2
	in al, dx
	and al, 0xF
	out dx, al
	mov dl, 0x72
	in al, dx
	and al, 0xF
	out dx, al


				; Remap PIC now

	mov al, 0x11
	out 0x20, al
	out 0xA0, al

	mov al, 0x20
	out 0x21, al 		; 2nd is targeted PIC IRQ address

	mov al, 0x28
	out 0xA1, al		; 2nd is targeted PIC IRQ address
	
	mov al, 0x04
	out 0x21, al

	mov al, 0x02
	out 0xA1, al

	mov al, 0x01
	out 0x21, al
	out 0xA1, al

				; Install IRQ's

	lea ecx, [GPF_IDT]
	mov al, 0xD
	call setidtvector

	lea ecx, [TSS_IDT]
	mov al, 0xA
	call setidtvector

	lea ecx, [IOP_IDT]
	mov al, 0x5
	call setidtvector
	
	lea ecx, [DevTimerIDT]
	mov al, 0x20
	call setidtvector

	lea ecx, [DevKeyboardIDT]
	mov al, 0x21
	call setidtvector	

	lea ecx, [Interruptor_IDT]
	mov al, 0x81
	call setidtvector

	lea ecx, [SysCallDispatcher_IDT]
	mov al, 0x80
	call setidtvector
