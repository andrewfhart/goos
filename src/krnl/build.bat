echo off

echo BUILDING GOOS KERNEL...

echo 1. Assembling gkstart using NASM

nasmw -f aout -o dbg\obj\gkstart.o gkstart.asm

echo 2. Compiling C source files using GCC
gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o main.o main.c
gcc -Wall -O3 -fomit-frame-pointer -nostdinc -fno-builtin -I./include -c -o dbg/obj/vga.o drivers/vga.c

rem This links all your files. Remember that as you add *.o files, you need to
rem add them after start.o. If you don't add them at all, they won't be in your kernel!

if not exist main.o goto cerror

cp main.o dbg\obj\main.o
del main.o

echo 3. Linking kernel using LD
ld -T gklink.ld -o dbg\gooskern.bin dbg\obj\gkstart.o dbg\obj\main.o dbg\obj\vga.o

echo 4. Creating Text Dump of Kernel
fhexdump dbg\gooskern.bin > dbg\gooskern.bin.txt


echo FINISHED.
pause
goto end
:cerror
pause
:end