=============
Mia Exokernel
=============
Oh, an operating system I was writing back in junior high. It's a far cry from targeted exokernel, being more of a monolith.

To compile it, compile mbrload.asm and kernel.asm with preferably nasm and call makeboot:

makeboot disk.img mbrload kernel

Doing so will produce floppy image `disk.img`
if the effective files are in current directory. 

How different is MiaOS from other OS'es?
========================================

MiaOS does not use paging. It uses exclusively segmenting for memory management. This yields itself to support theoretically bigger address spaces for programs.
