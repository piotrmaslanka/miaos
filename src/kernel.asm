
[ORG 0x00000000]
[BITS 32]

	%include "critic\setup.inc"		; Mia OS Preinitialize Directives



	mov [ds:MUIMANStart], dword 1
	mov [ds:MUIMANStart+8], dword 8
	mov [ds:MUIMANStart+12], dword 0x1000


	mov [ds:MUIMANStart+16], dword 2
	mov [ds:MUIMANStart+16+8], dword 0x12
	mov [ds:MUIMANStart+16+12], dword 0x1000

	mov [ds:0x12000], dword 0			; Keine Messagunginung


tester:
	

		;	EDI points to where fill it
	;	ECX shows how much 

	mov [ds:0x8000], dword 0x3
	mov [ds:0x8004], dword 0x9000					; Register first process
	mov [ds:0x8014], dword 0x10000					; Register second process
	mov [ds:0x8024], dword 0x11000					; Register second process
		

	mov [ds:0x9000], dword 0x1					; Register first thread
	mov [ds:0x9004], dword ldt
	mov [ds:0x9008], dword LDTSize

	mov [ds:0x10000], dword 0x1
	mov [ds:0x10004], dword ldt
	mov [ds:0x10008], dword LDTSize

	mov [ds:0x11000], dword 0x1
	mov [ds:0x11004], dword ldt
	mov [ds:0x11008], dword LDTSize




			; Configure the taskswitcher

	mov [ds:thread_stack], dword 0x9080		;	7ef
	mov [ds:act_process], dword 0x1			;	7f3
	mov [ds:max_thread], dword 0x1			;	7f7
	mov [ds:act_thread], dword 0x1			;	7fb
	mov [ds:proc_block], dword 0x9000		;	7ff
		

	mov ax, 100000b
	ltr ax

	mov eax, ldt
	mov ecx, LDTSize
	call construct_ldt_descriptor
	

ziomalu:
	sti
	mov esp, 0x9040 + 20
	push 1111b
	push 0x300000		
	pushf
	push 111b
	push testselend

	mov esp, 0x10040 + 20
	push 1111b
	push 0x300000		
	pushf
	push 111b
	push _repeat

	mov esp, 0x11040 + 20
	push 1111b
	push 0x300000		
	pushf
	push 111b
	push __repeat



testsel:

	push 1111b
	push 0x300000		; Stos na 3 MB, lolz
	pushf
	push 111b
	push testselend
	iret		

testselend:				; Moja wkompilowana aplikacja Ring3

	mov ax, 1111b
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ecx, 0x0fffff
costam:	
	loop costam

	lea ecx, [message]
	mov eax, 2
	int 80h

	lea ecx, [message_gen]
	mov eax, 2
	int 80h

	cmp eax, 0x12345678
	jnz testselend
	
	jmp near SysStop	
	hlt

message:
	dd 2
	dd 2
	dd 2
	dd 2
	dd 2
	dd 2
	dd 2
	dd 2

message_gen:
	dd 3
	dd 3
	dd 2
	dd 2
	dd 2
	dd 2
	dd 2
	dd 2


	%include "critic\console.inc"

	%include "critic\const.inc"		; System Constants
	%include "critic\gdt.inc"		; GDT Layout
	%include "critic\initf.inc"		; Preinitialize procedures
	%include "critic\devices.inc"		; Compiled device drivers //temp//
	%include "critic\muiman.inc"		; Memory-mapped file manager
	%include "critic\memman.inc"		; Memory manager
	%include "critic\tss.inc"		; Task State Segment
	%include "critic\taskldr.inc"		; Task stopping and resuming
	%include "critic\syscall.inc"		; System calls
	%include "critic\msg.inc"		; Messaging subsystem

	%include "critic\drivers\vga.inc"	; VGA


myidt:	
	dw IDTTableSize
	dd IDTTableAddr
;0x97