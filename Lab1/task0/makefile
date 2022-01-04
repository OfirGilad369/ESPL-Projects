all: task0

task0: main.o numbers.o add.o
	gcc -g -m32 -o main main.o numbers.o add.o

main.o: main.c
	gcc -g -m32 -c -o main.o main.c

numbers.o: numbers.c
	gcc -g -m32 -c -o numbers.o numbers.c

add.o: add.s
	nasm -g -f elf -o add.o add.s

.PHONY: clean

clean:
	rm -rf ./*.o main
