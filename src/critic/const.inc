
KbdInt dd 0


IDTTableAddr  equ 0x9F800
IDTTableSize  equ 0xA0000 - 0x9F800
GRAFTEXTSTART equ 0xB8000
StackStart    equ 0x9B800

MemoryMap equ 0x100000
MemoryLimit dd 0x0

MUIMANStart equ 0x100000

SystemCodeSeg equ 0x08
SystemDataSeg equ 0x10

VCCurrentCurpos  dw 160
KeybBufferSize dw 0
KeybBufferMaxSize equ 10

KeybBuffer: times KeybBufferMaxSize db 0

