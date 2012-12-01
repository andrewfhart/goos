echo off
echo PUBLISHING GOOS
echo 1. COMPILING SOURCE FILES
echo ========================================
cd src\boot
call publish.bat
cd ..\lib
call publish.bat
cd ..\krnl
call publish.bat
cd ..\..\
pause
echo 2. LINKING COMPILED OBJECTS
echo ========================================
echo - Creating gooskern.bin
ld -T gooslink.ld -o bin\krnl\gooskern.bin
fhexdump bin\krnl\gooskern.bin > src\krnl\dbg\gooskern.bin.txt
pause
echo 3. CREATING BINARY IMAGE
echo ========================================
echo - Creating goosimg.bin
imgwrite /I goosimg.bin /F bin\boot\goosboot.bin /S
imgwrite /I goosimg.bin /F bin\boot\sector2.bin /O 512 /S
imgwrite /I goosimg.bin /C /CC 512 /O 1024 /S
imgwrite /I goosimg.bin /F bin\krnl\gooskern.bin /O 1536 /S
pause
echo 4. PUBLISHING IMAGE TO Bochs
echo ========================================
echo - Writing goosimg.bin to 1.44
imgwrite /I c:\progra~1\bochs-~1.6\goos\1.44 /F goosimg.bin /S
echo FINISHED.
pause