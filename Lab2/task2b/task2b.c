#include <stdio.h> 
#include <ctype.h>
#include <string.h>

/* task 0 from lab1 - prints c to the standard output */
void echo_printer(char c) {
	if(c != '\n') {
		printf("%c ", c);
	}
}
/* task 1a from lab1 - print the ASCII code of c to the standard output*/
void ascii_printer(char c) {
	if(c != '\n') {
		printf("%d ", c);
	}
}
/* task 1b from lab1 – print the binary representation of c to the standard output */
void binary_printer(char c) {
	if(c != '\n') {
		for (int i = 7; i >= 0; i--) {
			printf("%d", c >> i & 1);
		}
		printf(" ");
	}
}
/* task 1c from lab1 – print c to the standard output in upper case */
void lower_to_upper_printer(char c) {
	if(c != '\n') {
		if(islower(c)) {
			printf("%c ", toupper(c));
		}
		else {
			printf("%c ", c);
		}
	}
}

void string_printer(char* str, void (*func_ptr) (char)){
	int i = 0;
	while (str[i] != '\0') {
		func_ptr(str[i]);
		i++;
	}
	printf("\n");
}

void string_reader(char* s){
	char input[10];
	strcpy(s, fgets(input, 10, stdin));
}

void bitwise_or(char* s){
  /* TODO: Complete during task2c */
}

int main(int argc, char **argv){
	char c[] = {'L','a','b','2','\0'};
	string_printer(c, ascii_printer);
}
