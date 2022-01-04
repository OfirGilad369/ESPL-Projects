all: addresses 

addresses: addresses.c
	gcc -g -m32 -c -o addresses.o addresses.c
	gcc -g -m32 -o addresses addresses.o
	
clean:
	rm -rf ./*.o addresses
