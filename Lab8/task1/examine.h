#define ELF_MAGIC "ELF"

int map_elf_file(char *elf_file_path, char **mapped_elf_file);
int get_elf_file_size(int file_descriptor);
void bytes_123_magic_number(char *reading_buffer, char *mapped_elf_file);
void load_elf_file_header(char *mapped_elf_file, Elf32_Ehdr *elf_file_header);

void print_elf_file_data_encoding_format(Elf32_Ehdr *elf_file_header);
void print_elf_file_entry_point(Elf32_Ehdr *elf_file_header);
void print_elf_file_section_header_table_offset(Elf32_Ehdr *elf_file_header);
void print_elf_file_section_header_entries_number(Elf32_Ehdr *elf_file_header);
Elf32_Shdr *load_elf_file_section_headers(Elf32_Ehdr *elf_file_header, char *mapped_elf_file);
void print_elf_file_section_header_entries_sizes(Elf32_Shdr *elf_file_section_headers, int section_headers_number);
void print_shstrtab_index(Elf32_Ehdr *elf_file_header);
