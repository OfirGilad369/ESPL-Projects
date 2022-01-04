all: task2a

task2a.o: task2a.asm
	nasm -g -O1 -f elf -o task2a.o task2a.asm

task2a: task2a.o
	ld -melf_i386 -o task2a task2a.o
	
clean:
	rm -f *.o task2a