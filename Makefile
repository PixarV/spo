ASM = nasm
ASM_FLAGS = -felf64 -g
UTIL = util_methods

all: try_forth

try_forth: try_forth.o 
	ld -o try_forth try_forth.o

try_forth.o: try_forth.asm macro.inc $(UTIL)/second.inc $(UTIL)/utils.inc 
	$(ASM) $(ASM_FLAGS) try_forth.asm -o try_forth.o

clean:
	rm -rf *.o try_forth
