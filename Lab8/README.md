# Lab 8: ELF
This lab is to help you understand how your executable/relocatable files look like, and what information they contain. We will parse the ELF file and extract useful information from it. In particular, we will access the data in the section header table, and in the symbol table. We will also learn to use the `mmap` system call.

## Important
This lab is written for 32bit machines. Some of the computers in the labs already run on a 64bit OS (use uname -a to see if the linux OS is 64bit or not). 32bit and 64bit machines have different instruction sets and different memory layout. Make sure to include the -m32 flag when you compile files, and to use the Elf32 data structures (and not the Elf64 ones).
In order to know if an executable file is compiled for 64bit or 32bit platform, you can use readelf, or the file command-line tool (for example: file `/bin/ls`).
