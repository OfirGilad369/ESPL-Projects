%.o: %.c
	gcc -g -m32 -Wall -ansi -c -o $@ $<

all: task4

task4: task4b.o task4a.o shell.o LineParser.o ShellHistory.o
	gcc -g -m32 -Wall -o task4 task4b.o task4a.o shell.o LineParser.o ShellHistory.o

clean:
	rm -f *.o task4
