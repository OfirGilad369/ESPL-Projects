all: task1b

task1b: task1b.o
	gcc -g -m32 -Wall -o char2ascii task1b.o

task1b.o: task1b.c
	gcc -g -m32 -c -o task1b.o task1b.c

.PHONY: clean

clean:
	rm -rf ./*.o char2ascii
