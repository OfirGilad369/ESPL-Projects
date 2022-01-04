%.o: %.c
	gcc -g -m32 -Wall -ansi -c -o $@ $<

all: task2

task2: task2.o examine.o
	gcc -g -m32 -Wall -o task2 task2.o examine.o

clean:
	rm -f *.o task2
