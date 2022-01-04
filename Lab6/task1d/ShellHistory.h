#define MAX_HISTORY_SIZE 10

typedef struct shellHistoryArray
{
	struct cmdLine *history_array[MAX_HISTORY_SIZE];
	int currFirstIndex;
	int lastIndex;
	int isFull;
} shellHistoryArray;

shellHistoryArray *createShellHistoryArray();

int addCmdLine(shellHistoryArray *sHistoryArray, cmdLine *pCmdLine);

void printHistory(shellHistoryArray *sHistoryArray);

void cmdLineToString(cmdLine *pCmdLine, char full_cmd_line[]);

void freeHistory(shellHistoryArray *sHistoryArray);

int historySize(shellHistoryArray *sHistoryArray);