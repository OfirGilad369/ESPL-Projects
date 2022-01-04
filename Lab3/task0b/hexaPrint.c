#include <stdio.h>

void PrintHex(unsigned char*, int);

int main(int argc, char **argv) {
    FILE * input;
    
    input = fopen(argv[1], "rb");
    fseek(input, 0L, SEEK_END);
    long int input_size = ftell(input);
    unsigned char buffer[input_size];
    rewind(input);
    fread(buffer, 1, input_size, input);
    PrintHex(buffer, input_size);
    
    return 0;
}

void PrintHex(unsigned char* buffer, int length) {
	for(int i = 0; i < length; i++) {
		printf("%02X ", buffer[i]);
	}
	printf("\n");
}
