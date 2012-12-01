rem --- Build Kernel ------------------------
echo - Compiling GOOS Kernel

rem compile gkstart using NASM
nasmw -f aout -o dbg\obj\gkstart.o gkstart.asm

rem compile c sources using GCC
gcc -Wall -O -I.\..\incl -c -o dbg\obj\main.o main.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\ci.o ci.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\cons.o core\cons.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\gdt.o  core\gdt.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\idt.o  core\idt.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\isrs.o core\isrs.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\irq.o  core\irq.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\pit.o  core\pit.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\kb.o   core\kb.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\cmos.o core\cmos.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\mm.o   core\mm.c


