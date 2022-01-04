%.o: %.c
	gcc -g -m32 -Wall -ansi -c -o $@ $<

all: task3

task3: task3.o shell.o LineParser.o ShellHistory.o
	gcc -g -m32 -Wall -o task3 task3.o shell.o LineParser.o ShellHistory.o

clean:
	rm -f *.o task3
