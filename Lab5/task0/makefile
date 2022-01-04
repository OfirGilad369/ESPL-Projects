all: prog

prog.o: prog.asm
	nasm -g -f elf -o prog.o prog.asm

task0.o: task0.c
	gcc -g -m32 -c -o task0.o task0.c

prog: task0.o prog.o
	gcc -g -m32 task0.o prog.o -o prog
