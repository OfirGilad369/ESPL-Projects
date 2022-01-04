all: task2

task2: task2.o
	gcc -g -m32 -Wall -o char2ascii task2.o

task2.o: task2.c
	gcc -g -m32 -c -o task2.o task2.c

.PHONY: clean

clean:
	rm -rf ./*.o char2ascii
