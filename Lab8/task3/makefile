%.o: %.c
	gcc -g -m32 -fno-pie -Wall -c -o $@ $<

all: patch task3 fibonacci

patch: patch.o 
	gcc -g -m32 -fno-pie -Wall -o patch patch.o

task3: task3.o
	gcc -g -m32 -fno-pie -Wall -o task3 task3.o

fibonacci: fibonacci.o
	gcc -g -m32 -fno-pie -Wall -o fibonacci fibonacci.o

clean:
	rm -f *.o patch task3 fibonacci
