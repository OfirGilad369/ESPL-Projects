all: task2

task2: task2.c
	gcc -g -m32 -c -o task2.o task2.c
	gcc -g -m32 -o bcmp task2.o

clean:
	rm -rf ./*.o bcmp
