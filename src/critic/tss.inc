

tss:

backlink:	dd 0
kernel_stack:	dd 0x4000
kernel_seg:	dw 0x10
		dw 0
ring1:		dd 0
		dd 0
ring2:		dd 0
		dd 0
pdpr:		dd 0
instr_ptr	dd 0
eflags		dd 0	;<eflags>
regeax:		dd 0
regecx:		dd 0
regedx:		dd 0
regebx:		dd 0
user_stack:	dd 0	;<stack>
regebp:		dd 0
regesi:		dd 0
regedi:		dd 0

reg_es:		dw 10h
		dw 0
reg_cs:		dw 10h
		dw 0
reg_ss:		dw 10h	;<stack>
		dw 0
reg_ds:		dw 10h
		dw 0
reg_fs:		dw 10h
		dw 0
reg_gs:		dw 10h
		dw 0
reg_ldt:	dw 0	;<lgt>
		dw 0
rl_est:		dd 0