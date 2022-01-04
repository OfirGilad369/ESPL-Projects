all: task2b

task2b.o: task2b.asm
	nasm -g -O1 -f elf -o task2b.o task2b.asm

task2b: task2b.o
	ld -melf_i386 -o task2b task2b.o
	
clean:
	rm -f *.o task2b
