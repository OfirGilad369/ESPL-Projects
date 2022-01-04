all: task1a

task1a: task1a.o
	gcc -g -m32 -Wall -o char2ascii task1a.o

task1a.o: task1a.c
	gcc -g -m32 -c -o task1a.o task1a.c

.PHONY: clean

clean:
	rm -rf ./*.o char2ascii
