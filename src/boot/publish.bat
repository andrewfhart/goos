echo - Compiling Boot Sectors

nasmw -f bin -o dbg\goosboot.bin goosboot.asm
nasmw -f bin -o dbg\sector2.bin sector2.asm
nasmw -f bin -o dbg\testkern.bin testkern.asm

fhexdump dbg\goosboot.bin > dbg\goosboot.bin.txt
fhexdump dbg\sector2.bin > dbg\sector2.bin.txt
fhexdump dbg\testkern.bin > dbg\testkern.bin.txt

cp dbg\goosboot.bin c:\osdev\GOOS\bin\boot\goosboot.bin
cp dbg\sector2.bin c:\osdev\GOOS\bin\boot\sector2.bin
cp dbg\testkern.bin c:\osdev\GOOS\bin\boot\testkern.bin


rem imgwrite /I c:\progra~1\bochs-~1.6\goos\1.44 /F dbg\goosboot.bin /O 0
rem imgwrite /I c:\progra~1\bochs-~1.6\goos\1.44 /F dbg\sector2.bin /O 512
rem imgwrite /I c:\progra~1\bochs-~1.6\goos\1.44 /F dbg\testkern.bin /O 1536

