#include <stdio.h>

extern int open(char *fileName, int access);
extern int close();

int main(int argc, char **argv)
{
	int res;
  
	if(argc != 2) {
		fprintf(stderr,"Usage: prog fileName\n");
		return 0;
	}
	
	res = open(argv[1], 0);
	
	printf("The corresponding file descriptor is %d\n", res);
	
	res = close();
	
	if(res >= 0) {
		printf("CLOSING DONE\n");
	}
	else {
		printf("CLOSING FAILED\n");
	}
	
	return 0;
}
