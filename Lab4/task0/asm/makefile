all: task0

my_cmp.o: my_cmp.asm
	nasm -g -f elf -o my_cmp.o my_cmp.asm

task0.o: task0.c
	gcc -g -m32 -c -o task0.o task0.c

task0: task0.o my_cmp.o
	gcc -g -m32 task0.o my_cmp.o -o task0
