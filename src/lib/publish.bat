echo - Compiling GOOS System libraries

gcc -Wall -O -I.\..\incl -c -o dbg\obj\printf.o printf.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\readline.o readline.c
gcc -Wall -O -I.\..\incl -c -o dbg\obj\strcmp.o strcmp.c
