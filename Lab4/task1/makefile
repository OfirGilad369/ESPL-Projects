all: task1

funcA.o: funcA.asm
	nasm -g -f elf -o funcA.o funcA.asm

task1.o: task1.c
	gcc -g -m32 -c -o task1.o task1.c

task1: task1.o funcA.o
	gcc -g -m32 task1.o funcA.o -o task1

.PHONY: clean

clean:
	rm -rf *.o task1
