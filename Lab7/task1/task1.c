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
    int child_status;
    pid_t cpid;
    /* io_files[0]='input_file', io_files[1]='output_file' */
    int io_files[2];
    
    if(pipe(io_files) == -1) {
		perror("pipe");  
		return;
	}
    
    cpid = fork();
    
    if(cpid == -1) {
        perror("fork");
        return;
    }
    if(cpid == 0) {
		io_redirection(parsed_cmd_line, io_files);
		/* execvp returns -1, and store message to errno if an error has occurred */
        child_status = execvp(parsed_cmd_line->arguments[0], parsed_cmd_line->arguments);
        if(child_status) {
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
    
    waitpid(cpid, &child_status, 0);
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
