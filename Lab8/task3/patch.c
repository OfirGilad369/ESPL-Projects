#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *source_file;
	int source_pos;
	int size;
	char *dest_file;
	int dest_pos;
	
	char *mapped_source_file;
	char *mapped_dest_file;
	
    int fdin, fdout;
    struct stat statbuf1;
	struct stat statbuf2;
    
    if(argc != 6) {
        fprintf(stderr,"Usage: patch source_file source_pos size dest_file dest_pos\n");
        return 1;
    }
    
    source_file = (char *)argv[1];
    source_pos = strtol(argv[2], NULL, 16);
    size = strtol(argv[3], NULL, 10);
    dest_file = (char *)argv[4];
    dest_pos = strtol(argv[5], NULL, 16);
    
	if((fdin = open(source_file, O_RDONLY)) < 0) { 
		printf("Can't open %s for reading\n", source_file);
		return 1;
	}

	if((fdout = open(dest_file, O_RDWR, 0x0777)) < 0) {
		printf("Can't create %s for writing\n", dest_file);
		return 1;
    }
    
    if(fstat(fdin,&statbuf1) < 0) {
		printf("fstat error\n");
		return 1;
	}
   
    if(fstat(fdout,&statbuf2) < 0) {
		printf("fstat error\n");
		return 1;
    }
    
	mapped_source_file = mmap(0, statbuf1.st_size, PROT_READ, MAP_SHARED, fdin, 0);
	mapped_source_file = mapped_source_file + source_pos;

	mapped_dest_file = mmap(0, statbuf2.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0);
	mapped_dest_file = mapped_dest_file + dest_pos;

	memcpy(mapped_dest_file, mapped_source_file, size);
    
	return 0;
}
