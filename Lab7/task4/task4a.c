#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "LineParser.h"
#include "task4a.h"

int **createPipes(int nPipes) {
    int **pipes = (int **)malloc(nPipes * sizeof(int *));
    int i;
    
    for(i = 0; i < nPipes; i++) {
        int *pipefd = (int *)malloc(2 * sizeof(int));
        pipe(pipefd);
        pipes[i] = pipefd;
    }

    return pipes;
}

void releasePipes(int **pipes, int nPipes) {
    int i;
    
    for(i = 0; i < nPipes; i++) {
        free(pipes[i]);
	}

    free(pipes);
}

int *leftPipe(int **pipes, cmdLine *pCmdLine) {
    if(pCmdLine->idx == 0) {
        return NULL;
	}

    return pipes[pCmdLine->idx - 1];
}

int *rightPipe(int **pipes, cmdLine *pCmdLine) {
    if(!pCmdLine->next) {
        return NULL;
	}

    return pipes[pCmdLine->idx];
}
