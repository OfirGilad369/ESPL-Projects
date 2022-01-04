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

#define ELF_MAGIC "ELF"


int map_elf_file(char *elf_file_path, char **mapped_elf_file) {
	int elf_file_size;
    int file_descriptor = open(elf_file_path, O_RDONLY, 0);
    
    if(file_descriptor == -1) {
        fprintf(stderr, "Failed to open: %s\n", elf_file_path);
        return -1;
    }
    
    elf_file_size = get_elf_file_size(file_descriptor);
    printf("File size: %d (bytes)\n", elf_file_size);
    (*mapped_elf_file) = (char *)mmap(NULL, elf_file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, file_descriptor, 0);
    
    if(*mapped_elf_file == MAP_FAILED) {
        fprintf(stderr, "Failed to Map: %s\n", elf_file_path);
        return -1;
    }

    return 0;
}

int get_elf_file_size(int file_descriptor) {
	off_t elf_file_size = lseek(file_descriptor, 0, SEEK_END);
	
	return elf_file_size;
}

void bytes_123_magic_number(char *reading_buffer, char *mapped_elf_file) {
	int i;
	
    strncpy(reading_buffer, mapped_elf_file + 1, 3);
    printf("Magic: ");
    
    for(i = 0; i < 3; i++) {
		printf("%x ", reading_buffer[i]);
	}
	
	printf("\n");
    reading_buffer[3] = '\0';
}

void load_elf_file_header(char *mapped_elf_file, Elf32_Ehdr *elf_file_header) {
    (*elf_file_header) = *(Elf32_Ehdr *)mapped_elf_file;
}

void print_elf_file_data_encoding_format(Elf32_Ehdr *elf_file_header) {
	switch (elf_file_header->e_ident[5]) {
		case ELFDATA2LSB:
			printf("Data: 2's complement, little endian\n");
			break;

		case ELFDATA2MSB:
			printf("Data: 2's complement, big endian\n");
			break;

		default:
			printf("Data: Unknown data format\n");
    }
}

void print_elf_file_entry_point(Elf32_Ehdr *elf_file_header) {
    printf("Entry point address: 0x%x\n", elf_file_header->e_entry);
}

void print_elf_file_section_header_table_offset(Elf32_Ehdr *elf_file_header) {
	printf("Start of section headers: %d (bytes into file)\n", elf_file_header->e_shoff);
}

void print_elf_file_section_header_entries_number(Elf32_Ehdr *elf_file_header) {
	printf("Number of section headers: %d\n", elf_file_header->e_shnum);
}

Elf32_Shdr *load_elf_file_section_headers(Elf32_Ehdr *elf_file_header, char *mapped_elf_file) {
    int i;
    int current_offset;
    Elf32_Shdr *section_headers = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr) *elf_file_header->e_shnum);
    
    for(i = 0; i < elf_file_header->e_shnum; i++) {
        current_offset = elf_file_header->e_shoff + i * sizeof(Elf32_Shdr);
        section_headers[i] = *(Elf32_Shdr *)(mapped_elf_file + current_offset);
    }

    return section_headers;
}

void print_elf_file_section_header_entries_sizes(Elf32_Shdr *elf_file_section_headers, int section_headers_number) {
	int i;
	
    for(i = 0; i < section_headers_number; i++) {
        printf("Section #%d size: %d (bytes)\n", i, elf_file_section_headers[i].sh_size);
	}
}

void print_shstrtab_index(Elf32_Ehdr *elf_file_header) {
	printf("shstrtab index: %d\n", elf_file_header->e_shstrndx);
}

