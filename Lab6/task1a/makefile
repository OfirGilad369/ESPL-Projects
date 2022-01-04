%.o: %.c
	gcc -g -m32 -Wall -ansi -c -o $@ $<

all: myshell

myshell: myshell.o LineParser.o
	gcc -g -m32 -Wall -o myshell myshell.o LineParser.o

clean:
	rm -f *.o myshell
