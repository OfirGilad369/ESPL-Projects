#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <limits.h>
#include <fcntl.h>

#include "shell.h"

void io_redirection(cmdLine *cmd_line, int io_files[]);
void close_io_files(int io_files[]);

void execute(cmdLine *parsed_cmd_line, shellHistoryArray *shell_history) {
	char const *error_string = "";
    int child_status1, child_status2;
    pid_t cpid1, cpid2;
    /* io_files[0]='input_file', io_files[1]='output_file' */
    int io_files[2];
	
	if(pipe(io_files) == -1) {
		perror("pipe");  
		return;
	}
	
	cpid1 = fork();
	
	if(cpid1 == -1) {
		perror("fork1");  
		return;
	}
	
	if(cpid1 == 0) { 
		io_redirection(parsed_cmd_line, io_files);
		/* execvp returns -1, and store message to errno if an error has occurred */
        child_status1 = execvp(parsed_cmd_line->arguments[0], parsed_cmd_line->arguments);
        if(child_status1) {
			/* Read message from errno */
            perror(error_string);
            fprintf(stderr, "%s", error_string);
            close_io_files(io_files);
            freeCmdLines(parsed_cmd_line);
            freeHistory(shell_history);
            _exit(EXIT_FAILURE);
        }
        close_io_files(io_files);
        freeCmdLines(parsed_cmd_line);
        freeHistory(shell_history);
        _exit(EXIT_SUCCESS);
	}
	
	if(!parsed_cmd_line->next) {
		waitpid(cpid1, &child_status1, 0);
		return;
	}
	
	close(io_files[1]);
	cmdLine *next_parsed_cmd_line = parsed_cmd_line->next;
	
	cpid2 = fork();
	
	if(cpid2 == -1) {
		perror("fork2");  
		return;
	}
	
	if(cpid2 == 0) { 
		io_redirection(next_parsed_cmd_line, io_files);
		/* execvp returns -1, and store message to errno if an error has occurred */
        child_status2 = execvp(next_parsed_cmd_line->arguments[0], next_parsed_cmd_line->arguments);
        if(child_status2) {
			/* Read message from errno */
            perror(error_string);
            fprintf(stderr, "%s", error_string);
            close_io_files(io_files);
            freeCmdLines(parsed_cmd_line);
            freeHistory(shell_history);
            close_io_files(io_files);
            _exit(EXIT_FAILURE);
        }
        close_io_files(io_files);
        freeCmdLines(parsed_cmd_line);
        freeHistory(shell_history);
        _exit(EXIT_SUCCESS);
	}
	close(io_files[0]);
	
	waitpid(cpid1, &child_status1, 0);
    waitpid(cpid2, &child_status2, 0);
}

void io_redirection(cmdLine *cmd_line, int io_files[]) {
	if(cmd_line->inputRedirect) {
        close(STDIN_FILENO);
        dup2(open(cmd_line->inputRedirect, O_RDONLY), STDIN_FILENO);
        close(io_files[0]);
    }
	
    if(cmd_line->outputRedirect) {
        close(STDOUT_FILENO);
        dup2(open(cmd_line->outputRedirect, O_WRONLY | O_CREAT, 0666), STDOUT_FILENO);
        close(io_files[1]);
    } 
    
    /* set cmd line to read from the previous cmd line output */
    if(cmd_line->idx != 0) {
        close(STDIN_FILENO);
        dup2(io_files[0], STDIN_FILENO);
        close(io_files[0]);
    }
    
    /* set cmd line to write to the next cmd line input */
    if(cmd_line->next) {
        close(STDOUT_FILENO);
        dup2(io_files[1], STDOUT_FILENO);
        close(io_files[1]);
    }
}

void close_io_files(int io_files[]) {
    if(io_files[0]) {
        close(io_files[0]);
	}
    
    if(io_files[1]) {
        close(io_files[1]);
	}
}

int main(int argc, char **argv) {
	display_prompt();
	return 0;
}
