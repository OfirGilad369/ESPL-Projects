%.o: %.c
	gcc -g -m32 -Wall -ansi -c -o $@ $<

all: examine

examine: examine.o
	gcc -g -m32 -Wall -o examine examine.o

clean:
	rm -f *.o examine
