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

#define SYMBOLS_PRINTING_FORMAT "[%3d] %-8x %-14d %-13s %-10s \n"
#define SYMBOLS_PRINTING_HEAD "[%3s] %-8s %-14s %-13s %-10s \n", "Num", "Value", "Section index", "Section name", "Symbol name"

void print_all_symbols_tables(char *mapped_elf_file, Elf32_Ehdr *elf_file_header, Elf32_Shdr *elf_file_section_headers, int elf_file_section_headers_number);
void print_single_symbols_table(char *mapped_elf_file, Elf32_Shdr *elf_file_current_section, Elf32_Shdr *elf_file_string_table, int elf_file_section_index, char *elf_file_section_name);

/* [index] value section_index section_name symbol_name */
void print_all_symbols_tables(char *mapped_elf_file, Elf32_Ehdr *elf_file_header, Elf32_Shdr *elf_file_section_headers, int elf_file_section_headers_number) {
    int i;
    char *elf_file_section_string_table = mapped_elf_file + elf_file_section_headers[elf_file_header->e_shstrndx].sh_offset;
    char *elf_file_current_section_name = NULL;
    Elf32_Shdr *elf_file_current_section = NULL;
    Elf32_Shdr *elf_file_current_string_table = NULL;

    for(i = 0; i < elf_file_section_headers_number; i++) {
        if(strncmp(".strtab", elf_file_section_string_table + elf_file_section_headers[i].sh_name, 7) == 0) {
            elf_file_current_string_table = &elf_file_section_headers[i];
            break;
        }
    }

    printf(SYMBOLS_PRINTING_HEAD);
    
    for(i = 0; i < elf_file_section_headers_number; i++) {
        elf_file_current_section = &elf_file_section_headers[i];
        elf_file_current_section_name = elf_file_section_string_table + elf_file_section_headers[i].sh_name;

        if(elf_file_current_section->sh_type != SHT_SYMTAB && elf_file_current_section->sh_type != SHT_DYNSYM) {
            continue;
		}
        
        print_single_symbols_table(mapped_elf_file, elf_file_current_section, elf_file_current_string_table, i, elf_file_current_section_name);
    }
}

void print_single_symbols_table(char *mapped_elf_file, Elf32_Shdr *elf_file_current_section, Elf32_Shdr *elf_file_string_table, int elf_file_section_index, char *elf_file_section_name) {
    int number_of_symbols = elf_file_current_section->sh_size / elf_file_current_section->sh_entsize;
    Elf32_Sym elf_file_current_symbol;

    int i;
    for(i = 0; i < number_of_symbols; i++) {
		elf_file_current_symbol = *(Elf32_Sym *)(mapped_elf_file + elf_file_current_section->sh_offset + i * elf_file_current_section->sh_entsize);

        printf(SYMBOLS_PRINTING_FORMAT,
               i,
               elf_file_current_symbol.st_value,
               elf_file_section_index,
               elf_file_section_name,
               mapped_elf_file + elf_file_current_symbol.st_name + elf_file_string_table->sh_offset);
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
    
    print_all_symbols_tables(mapped_elf_file, &elf_file_header, elf_file_section_headers, elf_file_header.e_shnum);
    
    free(elf_file_section_headers);
    
	return 0;
}
