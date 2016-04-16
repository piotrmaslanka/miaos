
setidtvector:

	;	ECX points to vector with IDT data
	;	AL picks interrupt number

	mov bl, 8	
	mul bl		    ; get the address in AX
	and eax, 0x0000FFFF

	mov ebx, [dword DS:ECX]
	mov edx, [dword DS:ECX+4]

	mov ebp, eax	

	mov dword [dword DS:IDTTableAddr+EBP], ebx
	mov dword [dword DS:IDTTableAddr+EBP+4], edx

	ret


clearmemory:
	;	EDI points to where fill it
	;	ECX shows how much 
	
	xor eax, eax
	rep stosb
	ret