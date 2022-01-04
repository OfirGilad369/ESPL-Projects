#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int fib(int limit) {
	int first, second, result;
	first = 0; 
	second = 1;
	if(limit < 1) {
		return -1;
	}
	while(second < limit) {
		result = first + second;
		first = second;
		second = result;
	}	
	return first;
}

int main(int argc, char **argv) {
	int limit;
	
	if(argc != 2) {
        fprintf(stderr,"Usage: task3 limit\n");
        return 1;
    }
    
    limit = atoi(argv[1]);
    printf("fib result: %d\n", fib(limit));
    
	return 0;
}
