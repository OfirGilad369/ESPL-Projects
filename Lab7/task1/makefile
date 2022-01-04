%.o: %.c
	gcc -g -m32 -Wall -ansi -c -o $@ $<

all: task1

task1: task1.o shell.o LineParser.o ShellHistory.o
	gcc -g -m32 -Wall -o task1 task1.o shell.o LineParser.o ShellHistory.o

clean:
	rm -f *.o task1
