%.o: %.c
	gcc -g -m32 -Wall -ansi -c -o $@ $<

all: task2

task2: task2.o
	gcc -g -m32 -Wall -o mypipeline task2.o

clean:
	rm -f *.o mypipeline
