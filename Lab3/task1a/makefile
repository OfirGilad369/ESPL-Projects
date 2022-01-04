all: task1a

task1a: task1a.c
	gcc -g -m32 -c -o task1a.o task1a.c
	gcc -g -m32 -o bcmp task1a.o

clean:
	rm -rf ./*.o bcmp
