#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

#include "LineParser.h"
#include "ShellHistory.h"

void parse_user_input(char user_input[], shellHistoryArray *shell_history);
void execute(cmdLine *parsed_cmd_line);
void change_direction(cmdLine *parsed_cmd_line);
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
	
	shellHistoryArray *shell_history = createShellHistoryArray();
	
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
		
		parse_user_input(user_input, shell_history);        
	}
	freeHistory(shell_history);
	system("clear");
}

void parse_user_input(char user_input[], shellHistoryArray *shell_history) {
	cmdLine *parsed_cmd_line = parseCmdLines(user_input);
	addCmdLine(shell_history, parsed_cmd_line);
	handle_thilda_in_cmd_line(parsed_cmd_line);
	
	if(strcmp(parsed_cmd_line->arguments[0], "cd") == 0) {
		change_direction(parsed_cmd_line);
	}
	else if(strcmp(parsed_cmd_line->arguments[0], "history") == 0) {
		printHistory(shell_history);
	}
	else {
		execute(parsed_cmd_line);
	}
	freeCmdLines(parsed_cmd_line);
}

void execute(cmdLine *parsed_cmd_line) {
	char error_string[1024];
    int child_status;
    pid_t cpid;
    
    cpid = fork();
    
    if(cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(cpid == 0) {
		/* execvp returns -1, and store message to errno if an error has occurred */
        child_status = execvp(parsed_cmd_line->arguments[0], parsed_cmd_line->arguments);
        if(child_status) {
			/* Read message from errno */
            perror(error_string);
            fprintf(stderr, "%s", error_string);
            _exit(EXIT_FAILURE);
        }
        _exit(EXIT_SUCCESS);
    }
    else {
        if(parsed_cmd_line->blocking) {
			waitpid(cpid, &child_status, 0);
		}
    }
}

void change_direction(cmdLine *parsed_cmd_line) {
	char error_string[1024];
	char full_path_name[2048];
	int full_path_name_index = 0;
	int current_arg_index = 1;
	int status;
	
	if(parsed_cmd_line->argCount == 1) {
        status = chdir(getenv("HOME"));
    }
    else {
		char *last_argument = parsed_cmd_line->arguments[parsed_cmd_line->argCount - 1];
		int last_argument_length = strlen(last_argument);
		
		if(parsed_cmd_line->arguments[1][0] == '"' && last_argument[last_argument_length - 1] == '"') {
			while(current_arg_index < parsed_cmd_line->argCount) {
				char *current_string = parsed_cmd_line->arguments[current_arg_index];
				int arg_size = strlen(current_string);
				int i;
				
				for(i = 0; i < arg_size; i++) {
					if((current_arg_index == 1 && i == 0) || (current_arg_index == parsed_cmd_line->argCount - 1 && i == arg_size - 1)) {
						continue;
					}
					full_path_name[full_path_name_index] = parsed_cmd_line->arguments[current_arg_index][i];
					full_path_name_index++;
				}
				full_path_name[full_path_name_index] = ' ';
				full_path_name_index++;
				current_arg_index++;
			}
			full_path_name_index--;
			full_path_name[full_path_name_index] = '\0';
			status = chdir(full_path_name);
        }
        else if(parsed_cmd_line->argCount == 2) {
			status = chdir(parsed_cmd_line->arguments[1]);
		}
        else {
			status = chdir("");
		}
    }
    
    if(status) {
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
