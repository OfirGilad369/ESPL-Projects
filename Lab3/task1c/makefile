all: task1c

task1c: task1c.c
	gcc -g -m32 -c -o task1c.o task1c.c
	gcc -g -m32 -o bcmp task1c.o

clean:
	rm -rf ./*.o bcmp
