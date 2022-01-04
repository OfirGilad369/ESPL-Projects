#include <stdio.h>
#include <string.h>

void case_controller(int, FILE *, FILE *);
void decimal_ascii(FILE *, FILE *);
void binary_ascii(FILE *, FILE *);

int main(int argc, char **argv) {
	FILE * input=stdin;
	FILE * output=stdout;
	int case_number = 0;
	
	if(argc >= 2 && strcmp(argv[1],"-b")==0) {
		case_number = 1;
	}
    
    case_controller(case_number, input, output);
	
	fclose(output);
	fclose(input);
	
	return 0;
}

void case_controller(int case_number, FILE * input, FILE * output) {
	if(case_number == 0) {
		decimal_ascii(input, output);
	}
    else if(case_number == 1) {
		binary_ascii(input, output);
	}
}

void decimal_ascii(FILE * input, FILE * output) {
	char c;
	
	c=fgetc(input);
	
	while(c != EOF) {
		if(c!='\n') {
			fprintf(output, "%d ", c);
		}
		else {
			fprintf(output, "\n");
		}
		c=fgetc(input);
	}
}

void binary_ascii(FILE * input, FILE * output) {
	char c;
	int number[8];
	
	c=fgetc(input);
		
	while(c != EOF) {
		if(c!='\n') {
			for(int i = 7; i >= 0; i--) {
				number[i] = (c >> i) & 1 ? 1 : 0;
			}
			for(int i = 7; i >= 0; i--) {
				fprintf(output, "%d", number[i]);
			}
			fprintf(output, " ");
		}
		else {
			fprintf(output, "\n");
		}
		c=fgetc(input);
	}
}
