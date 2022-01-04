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
#include "task4a.h"

void io_redirection(cmdLine *cmd_line, int io_files[], int **pipes);
void close_io_files(int io_files[]);
int pipeline_length(cmdLine *parsed_cmd_line);
int execute_cmd_line(cmdLine *parsed_cmd_line, int **pipes, shellHistoryArray *shell_history);

void execute(cmdLine *parsed_cmd_line, shellHistoryArray *shell_history) {
    int child_status;
    pid_t cpid;
    cmdLine *current_cmd_line = parsed_cmd_line;
	
	int num_of_cmd_lines = pipeline_length(parsed_cmd_line);
	int **pipes = createPipes(num_of_cmd_lines - 1);
    int *cpids = (int *)malloc(sizeof(int) * num_of_cmd_lines);
    int cpids_counter = 0;
    char blocking_status = 0;
	
	int i;
	for (i = 0; i < num_of_cmd_lines; i++) {
		cpid = fork();
		
		if(cpid == -1) {
			perror("fork");  
			return;
		}
	
		if(cpid == 0) { 
			child_status = execute_cmd_line(current_cmd_line, pipes, shell_history);
			/* free all allocated memory */
			freeCmdLines(parsed_cmd_line);
			freeHistory(shell_history);
			releasePipes(pipes, num_of_cmd_lines - 1);
			free(cpids);
			/* exit */
			_exit(child_status);
		}
	
		if(current_cmd_line->next) {
            close(pipes[i][1]);
		}
        if(current_cmd_line->idx) {
            close(pipes[i - 1][0]);
		}
		
		cpids[cpids_counter] = cpid;
		cpids_counter++;
		blocking_status |= current_cmd_line->blocking;
        current_cmd_line = current_cmd_line->next;

        waitpid(cpids[i], &child_status, 0);
        if(child_status == -1) {
            releasePipes(pipes, num_of_cmd_lines - 1);
            free(cpids);
            return;
        }
	}
	
	if(blocking_status) {
        for(i = 0; i < cpids_counter; i++) {
            waitpid(cpids[i], &child_status, 0);
        }
    }
    
    releasePipes(pipes, num_of_cmd_lines - 1);
    free(cpids);
}

void io_redirection(cmdLine *cmd_line, int io_files[], int **pipes) {
	int *left_pipe, *right_pipe;
	
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
    
    
    left_pipe = leftPipe(pipes, cmd_line);
    
    if(left_pipe) {
        close(STDIN_FILENO);
        dup2(left_pipe[0], STDIN_FILENO);
        close(left_pipe[0]);
    }
    
    right_pipe = rightPipe(pipes, cmd_line);
    
    if(right_pipe) {
        close(STDOUT_FILENO);
        dup2(right_pipe[1], STDOUT_FILENO);
        close(right_pipe[1]);
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

int pipeline_length(cmdLine *parsed_cmd_line) {
    int counter = 0;
    while(parsed_cmd_line) {
        counter++;
        parsed_cmd_line = parsed_cmd_line->next;
    }
    return counter;
}

int execute_cmd_line(cmdLine *parsed_cmd_line, int **pipes, shellHistoryArray *shell_history) {
    int status;
    char const *error_string = "";
    int io_files[2];
	
	if(pipe(io_files) == -1) {
		perror("pipe");  
		status = -1;
	}
	
	io_redirection(parsed_cmd_line, io_files, pipes);
	/* execvp returns -1, and store message to errno if an error has occurred */
    status = execvp(parsed_cmd_line->arguments[0], parsed_cmd_line->arguments);
    
    if(status) {
		/* Read message from errno */
		perror(error_string);
		fprintf(stderr, "%s", error_string);
	}
	close_io_files(io_files);
	
	return status;
}

int main(int argc, char **argv) {
	display_prompt();
	return 0;
}
