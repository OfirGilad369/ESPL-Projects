#include <stdio.h>

void decimal_ascii(FILE *, FILE *);

int main(int argc, char **argv) {
	FILE * input=stdin;
	FILE * output=stdout;
	
	decimal_ascii(input, output);
	
	fclose(output);
	fclose(input);
	
	return 0;
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
