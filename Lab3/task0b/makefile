all: hexaPrint

hexaPrint: hexaPrint.c
	gcc -g -m32 -c -o hexaPrint.o hexaPrint.c
	gcc -g -m32 -o hexaPrint hexaPrint.o

clean:
	rm -rf ./*.o hexaPrint
