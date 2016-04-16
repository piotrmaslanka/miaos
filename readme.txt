Oh, an operating system I was writing back in junior high.

To compile it, compile mbrload.asm and kernel.asm and call makeboot:

makeboot disk.img mbrload kernel

Doing so will produce floppy image
if the effective files are in current directory. 
