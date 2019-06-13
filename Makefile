ASM = nasm
AFLAGS = -felf64

all: load 


load: forth.o util.o
		ld forth.o util.o -o main

forth.o:  forth.asm
		$(ASM) $(AFLAGS) forth.asm -o fortrness.o

util.o: util.asm
		$(ASM) $(AFLAGS) util.asm -o util.o

clean: 
	rm -rf *.o 

