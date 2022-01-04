#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

#include "LineParser.h"

void parse_user_input(char user_input[]);
void execute(cmdLine *parsed_cmd_line);
void handle_thilda_in_cmd_line(cmdLine *cmd_line);
int replace_path(char *current_path, char *sub_path_to_replace, char *replacement_path);

char *MYSHELL_PATH_PRINTER = "MS ~%s> ";

void display_prompt() {
	char path_name[PATH_MAX];
	char user_input[2048];
	system("clear");
	printf("\033[1;31m");
	printf("MyShell \nCopyright (C) Ofir and Mickey. All rights reserved.\n\n");
	printf("\033[0m");
	
	while(1) {
		getcwd(path_name, PATH_MAX);
		printf("\033[1;31m");
		printf(MYSHELL_PATH_PRINTER, path_name);
		printf("\033[0m");
		fgets(user_input, 2048, stdin);
		
		if(strcmp(user_input, "quit\n") == 0) {
            break;
		}
		/* Skip reading user input if it's blank */
		else if(strcmp(user_input, "\n") == 0) {
            continue;
		}
		
		parse_user_input(user_input);        
	}
	system("clear");
}

void parse_user_input(char user_input[]) {
	cmdLine *parsed_cmd_line = parseCmdLines(user_input);
	handle_thilda_in_cmd_line(parsed_cmd_line);
	execute(parsed_cmd_line);
	freeCmdLines(parsed_cmd_line);
}

void execute(cmdLine *parsed_cmd_line) {
	char error_string[1024];
    int status;
    
    status = execvp(parsed_cmd_line->arguments[0], parsed_cmd_line->arguments);
    if(status) {
		/* Read message from errno */
        perror(error_string);
        fprintf(stderr, "%s", error_string);
    }
}

void handle_thilda_in_cmd_line(cmdLine *cmd_line) {
    char argument_copy[2048];
    int i;
	
    for (i = 0; i < cmd_line->argCount; i++) {
		char *current_argument = cmd_line->arguments[i];
		
        memset(argument_copy, '\0', 0);
        strcpy(argument_copy, current_argument);
        
        if(replace_path(argument_copy, "~", getenv("HOME")) == 0) {
            replaceCmdArg(cmd_line, i, argument_copy);
		}
    }
}

int replace_path(char *current_path, char *sub_path_to_replace, char *replacement_path) {
    char *first_occurrence_pointer = strstr(current_path, sub_path_to_replace);
    
    /* The sub path to replace wasn't found */
    if(!first_occurrence_pointer) {
        return -1;
	}
    char replacement_path_buffer[1024];
    memset(replacement_path_buffer, '\0', 0);
	
	/* The sub path to replace was found at the start of current path */
    if (current_path == first_occurrence_pointer) {
        strcpy(replacement_path_buffer, replacement_path);
        strcat(replacement_path_buffer, first_occurrence_pointer + strlen(sub_path_to_replace));
    }
    else {
        strncpy(replacement_path_buffer, current_path, strlen(current_path) - strlen(first_occurrence_pointer));
        strcat(replacement_path_buffer, replacement_path);
        strcat(replacement_path_buffer, first_occurrence_pointer + strlen(sub_path_to_replace));
    }
    
    memset(current_path, '\0', strlen(current_path));
    strcpy(current_path, replacement_path_buffer);
    return 0;
}

int main(int argc, char **argv) {
	display_prompt();
	return 0;
}
