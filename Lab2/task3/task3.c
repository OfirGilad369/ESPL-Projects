#include <stdio.h> 
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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
	while(str[i] != '\0') {
		func_ptr(str[i]);
		i++;
	}
	printf("\n");
}

void string_reader(char* s){
    fgets(s, 10, stdin);
}

void bitwise_or(char* s){
	int i = 1;
	int currentAscii = (int)s[0];
	while(s[i] != '\0' && s[i] != '\n') {
		currentAscii = currentAscii | (int)s[i];
		i++;
	}
	binary_printer(currentAscii);
	printf("\n");
}

struct fun_desc {
	char *name;
	void (*fun)(char);
};

void menu() {
	char *user_input = malloc(10);
	int type_print;
	struct fun_desc fun_array[] = {
		{"echo printer", echo_printer},
		{"ASCII printer", ascii_printer},
		{"binary printer", binary_printer},
		{"lower to upper", lower_to_upper_printer},
        {"bitwise or", NULL}
	};

	printf("Please enter a string (0<size<=10):\n");
	string_reader(user_input);
	printf("Please choose printer type:\n");
	for(int i = 0; i < 5; i++) {
        printf("%d) %s\n", i, fun_array[i].name);
	}
	printf("\n");
	printf("Option: ");
	while(scanf("%d", &type_print) != EOF) {
		if(type_print < 0 || type_print > 4) {
			printf("Please select 1-4 or CTRL-D to exit.\n");
		}
		else {
			if (type_print == 4) {
				bitwise_or(user_input);
			}
			else {
				string_printer(user_input, fun_array[type_print].fun);
			}
		}
		printf("Option: ");
	}
	printf("DONE.\n");
}

int main(int argc, char **argv){
	menu();
}
