#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include <elf.h>
#include "examine.h"

#define SECTIONS_PRINTING_FORMAT "[%2d] %-20s %-8x %-6x %-8x \n"
#define SECTIONS_PRINTING_HEAD "[%2s] %-20s %-8s %-6s %-8s \n", "Nr", "Name", "Addr", "Off", "Size"

void print_all_sections(char *mapped_elf_file, Elf32_Ehdr *elf_file_header, Elf32_Shdr *elf_file_section_headers, int elf_file_section_headers_number);

void print_all_sections(char *mapped_elf_file, Elf32_Ehdr *elf_file_header, Elf32_Shdr *elf_file_section_headers, int elf_file_section_headers_number) {
    int i;
    char *section_str_table = mapped_elf_file + elf_file_section_headers[elf_file_header->e_shstrndx].sh_offset;
	
    printf(SECTIONS_PRINTING_HEAD);

    for(i = 0; i < elf_file_section_headers_number; i++) {
		printf(SECTIONS_PRINTING_FORMAT, i, 
		section_str_table + elf_file_section_headers[i].sh_name, 
		elf_file_section_headers[i].sh_addr, 
		elf_file_section_headers[i].sh_offset, 
		elf_file_section_headers[i].sh_size);
    }
}

int main(int argc, char **argv) {
	char *elf_file_path;
	char *mapped_elf_file;
    Elf32_Ehdr elf_file_header;
    char reading_buffer[100];
    
    if(argc != 2) {
        fprintf(stderr,"Usage: examine INFILE\n");
        return 1;
    }
    
    elf_file_path = (char *)argv[1];
    
    if(map_elf_file(elf_file_path, &mapped_elf_file) != 0) {
		return 1;
	}
	
	bytes_123_magic_number(reading_buffer, mapped_elf_file);
	
	if (strncmp(ELF_MAGIC, reading_buffer, 3) != 0) {
        printf("File: '%s' is not in elf format!\n", elf_file_path);
        return 1;
    }
	
	load_elf_file_header(mapped_elf_file, &elf_file_header);
    Elf32_Shdr *elf_file_section_headers = load_elf_file_section_headers(&elf_file_header, mapped_elf_file);
    
    print_all_sections(mapped_elf_file, &elf_file_header, elf_file_section_headers, elf_file_header.e_shnum);
    
    free(elf_file_section_headers);
    
	return 0;
}
