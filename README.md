Mia Exokernel
=============
Oh, an operating system I was writing back in junior high. It's a far cry from targeted exokernel, being more of a monolith.

To compile it, compile mbrload.asm and kernel.asm with preferably nasm and call makeboot:

makeboot disk.img mbrload kernel

Doing so will produce floppy image `disk.img`
if the effective files are in current directory. 
