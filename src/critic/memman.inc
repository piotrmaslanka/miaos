
memman_needswap:
	cli
	hlt				; Kernel panic: out of memory
	ret


memman_allocate_single_block:				

;		Memory Manager  -  Allocate Single Memory Block
;	Restrictions: DS - must be System Segment
;	Return/Input: EAX - address of allocated memory block


	mov eax, MemoryMap
	mov ecx, [ds:MemoryLimit]
memman_asb_loop01:
	cmp dword [ds:eax], 0xFFFFFFFF
	jnz memman_asb_found1
	add eax, 4
	loop memman_asb_loop01
	call memman_needswap			; Get some memory space
	jmp memman_allocate_single_block	; Reinvoke procedure
memman_asb_found1
	mov edx, 1
	mov ecx, 31
	xor ebp, ebp
memman_asb_loop02:
	test [ds:eax], edx		
        jnz memman_asb_jump01		
	mov [ds:eax], edx
	jmp memman_asb_end
memman_asb_jump01:
	shl edx, 1
	inc ebp
	loop memman_asb_loop02
memman_asb_end:
	sub eax, MemoryMap
	shl eax, 15
	shl ebp, 12
	add eax, ebp
	ret

memman_deallocate_single_block
	; EAX address
	; DS system segment =]

	shr eax, 15		;	GetTableEntryAddress()
	mov ecx, eax		;	prepare to get
	shr eax, 5			; EAX pointer do wpisu
	and ecx, 11111b		; Getto byte
	mov ebx, 1
	shr ebx, cl
	mov edx, [ds:eax+MemoryMap]
	mov esi, 0xFFFFFFFF
	sub esi, ebx
	and edx, esi
	mov [ds:eax+MemoryMap], esi
	ret

; ECX - the under5 part
; EAX - the over5 part
	







