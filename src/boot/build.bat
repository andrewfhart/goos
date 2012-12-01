echo off
echo Compiling GOOS BootSector...
echo 1. Compiling with NASM
nasmw -f bin -o dbg\goosboot.bin goosboot.asm
nasmw -f bin -o dbg\sector2.bin sector2.asm
fhexdump /F dbg\goosboot.bin > dbg\goosboot.bin.txt
fhexdump /F dbg\sector2.bin > dbg\sector2.bin.txt
pause

