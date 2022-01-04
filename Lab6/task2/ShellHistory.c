#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "LineParser.h"
#include "ShellHistory.h"

#ifndef NULL
    #define NULL 0
#endif

#define FREE(X) if(X) free((void*)X)

shellHistoryArray *createShellHistoryArray() {
	shellHistoryArray* sHistoryArray = (shellHistoryArray*)malloc(sizeof(shellHistoryArray));
	sHistoryArray->currFirstIndex = 0;
	sHistoryArray->lastIndex = -1;
	sHistoryArray->isFull = 0;
	return sHistoryArray;
}

int addCmdLine(shellHistoryArray * sHistoryArray, cmdLine *pCmdLine) {
	int currentIndex = sHistoryArray->currFirstIndex;
	char cmd_line_string_buffer[2048];
	cmdLineToString(pCmdLine, cmd_line_string_buffer);
	
	if(sHistoryArray->isFull == 0) {
		strcpy(sHistoryArray->history_array[sHistoryArray->lastIndex + 1], cmd_line_string_buffer);
		sHistoryArray->lastIndex = sHistoryArray->lastIndex + 1;
		if (sHistoryArray->lastIndex == MAX_HISTORY_SIZE - 1) {
			sHistoryArray->isFull = 1;
		}
		return 0;
	}
	
	strcpy(sHistoryArray->history_array[currentIndex], cmd_line_string_buffer);
	if (currentIndex == MAX_HISTORY_SIZE - 1) {
		sHistoryArray->currFirstIndex = 0;
		sHistoryArray->lastIndex = MAX_HISTORY_SIZE - 1;
		return 0;
	}
	if (sHistoryArray->lastIndex == MAX_HISTORY_SIZE - 1) {
		sHistoryArray->currFirstIndex = sHistoryArray->currFirstIndex + 1;
		sHistoryArray->lastIndex = 0;
		return 0;
	}
	sHistoryArray->currFirstIndex = sHistoryArray->currFirstIndex + 1;
	sHistoryArray->lastIndex = sHistoryArray->lastIndex + 1;
	return 0;
}

void printHistory(shellHistoryArray *sHistoryArray) {
	if (sHistoryArray->isFull == 0) {
		int size_of_history = sHistoryArray->lastIndex + 1;
		int i;
		for(i = 0; i < size_of_history; i++) {
			printf(" %d  %s\n", i, sHistoryArray->history_array[i]);
		}
	} 
	else {
		int i, j;
		j = 0;
		
		for (i = sHistoryArray->currFirstIndex ; i < MAX_HISTORY_SIZE; i++) {
			printf(" %d  %s\n", j, sHistoryArray->history_array[i]);
			j = j + 1;
		}
		for (i = 0; i < sHistoryArray->currFirstIndex; i++) {
			printf(" %d  %s\n", j, sHistoryArray->history_array[i]);
			j = j + 1;
		}
	}
}

void cmdLineToString(cmdLine *pCmdLine, char full_cmd_line[]) {
	int full_cmd_line_index = 0;
	int current_arg_index = 0;
	
	while(current_arg_index < pCmdLine->argCount) {
		char *current_string = pCmdLine->arguments[current_arg_index];
		int arg_size = strlen(current_string);
		int i;
				
		for(i = 0; i < arg_size; i++) {
			full_cmd_line[full_cmd_line_index] = pCmdLine->arguments[current_arg_index][i];
			full_cmd_line_index++;
		}
		full_cmd_line[full_cmd_line_index] = ' ';
		full_cmd_line_index++;
		current_arg_index++;
	}
	full_cmd_line_index--;
	full_cmd_line[full_cmd_line_index] = '\0';
}

void freeHistory(shellHistoryArray *sHistoryArray) {
	FREE(sHistoryArray);
}

int historySize(shellHistoryArray *sHistoryArray) {
	if (sHistoryArray->isFull) {
		return MAX_HISTORY_SIZE;
	}
	else {
		return sHistoryArray->lastIndex + 1;
	}
}
