%.o: %.asm
	nasm -g -O1 -f elf -o $@ $<

%.o: %.c
	gcc -m32 -g -nostdlib -fno-stack-protector -c -o $@ $<

all: lwca

lwca: lwc.o task1b.o
	ld  -melf_i386 -o $@ $^

clean:
	rm -f *.o lwca
