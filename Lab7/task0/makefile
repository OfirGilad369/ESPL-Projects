%.o: %.c
	gcc -g -m32 -Wall -ansi -c -o $@ $<

all: mypipe

mypipe: mypipe.o
	gcc -g -m32 -Wall -o mypipe mypipe.o

clean:
	rm -f *.o mypipe
