all: task1b

task1b: task1b.c
	gcc -g -m32 -c -o task1b.o task1b.c
	gcc -g -m32 -o bcmp task1b.o

clean:
	rm -rf ./*.o bcmp
