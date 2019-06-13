ASM = nasm
AFLAGS = -felf64

all: load 


load: forthress.o util.o
		ld forthress.o util.o -o main

forthress.o:  forthress.asm
		$(ASM) $(AFLAGS) forthress.asm -o fortrness.o

util.o: util.asm
		$(ASM) $(AFLAGS) util.asm -o util.o

clean: 
	rm -rf *.o 

