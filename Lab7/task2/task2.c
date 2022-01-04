#include <sys/types.h>  
#include <sys/wait.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>


int main(int argc, char **argv) {
	int pipefd[2];  
	pid_t cpid1, cpid2;
	int status1, status2;  
	char *cmd1[3] = {"ls", "-l", NULL};
    char *cmd2[4] = {"tail", "-n", "2", NULL};
    int print_indicator = 0;
	
	if(argc > 1) {
		if(strcmp(argv[1],"-d")==0) {
			print_indicator = 1;
		}
	}
	
	if(pipe(pipefd) == -1) {
		perror("pipe");  
		exit(EXIT_FAILURE);
	}
	
	if(print_indicator) {
		fprintf(stderr, "%d>forking...\n", getpid());
	}
	
	cpid1 = fork();
	
	if(cpid1 == -1) {
		perror("fork1");  
		exit(EXIT_FAILURE);
	}
	
	if(cpid1 == 0) { 
		if(print_indicator) {
			fprintf(stderr, "%d>redirecting stdout to the write end of the pipe...\n", getpid());
		}
		
		close(STDOUT_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        if(print_indicator) {
			fprintf(stderr, "%d>going to execute cmd: 'ls -l'\n", getpid());
		}
		
        execvp("ls", cmd1);
		_exit(EXIT_SUCCESS);
	}
	else {
		if(print_indicator) {
			fprintf(stderr, "%d>created process with id: %d\n", getpid(), cpid1);
		}
	}
	
	if(print_indicator) {
		fprintf(stderr, "%d>closing the write end of the pipe...\n", getpid());
	}
	close(pipefd[1]);
	
	if(print_indicator) {
		fprintf(stderr, "%d>forking...\n", getpid());
	}
	
	cpid2 = fork();
	
	if(cpid2 == -1) {
		perror("fork2");  
		exit(EXIT_FAILURE);
	}
	
	if(cpid2 == 0) { 
		if(print_indicator) {
			fprintf(stderr, "%d>redirecting stdout to the read end of the pipe...\n", getpid());
		}
		
		close(STDIN_FILENO);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        
        if(print_indicator) {
			fprintf(stderr, "%d>going to execute cmd: 'tail -n 2'\n", getpid());
		}
        
		execvp("tail", cmd2);
		_exit(EXIT_SUCCESS);
	}
	else {
		if(print_indicator) {
			fprintf(stderr, "%d>created process with id: %d\n", getpid(), cpid2);
		}
	}
	
	if(print_indicator) {
		fprintf(stderr, "%d>closing the read end of the pipe...\n", getpid());
	}
	close(pipefd[0]);
	
	if(print_indicator) {
		fprintf(stderr, "%d>waiting for child processes to terminate...\n", getpid());
	}
	
	waitpid(cpid1, &status1, 0);
    waitpid(cpid2, &status2, 0);
	
	if(print_indicator) {
		fprintf(stderr, "%d>exiting...\n", getpid());
	}
	
	return 0;
}
