all: task1c

task1c: task1c.o
	gcc -g -m32 -Wall -o char2ascii task1c.o

task1c.o: task1c.c
	gcc -g -m32 -c -o task1c.o task1c.c

.PHONY: clean

clean:
	rm -rf ./*.o char2ascii
