#include "ShellHistory.h"

#define MYSHELL_PATH_PRINTER "MS ~%s> "

void display_prompt();

void parse_user_input(char user_input[], shellHistoryArray *shell_history);

void change_directory(cmdLine *parsed_cmd_line);

void handle_thilda_in_cmd_line(cmdLine *cmd_line);

int replace_path(char *current_path, char *sub_path_to_replace, char *replacement_path);

int handle_history_cmd_line(cmdLine **parsed_cmd_line_pointer, shellHistoryArray *shell_history);


void execute(cmdLine *parsed_cmd_line, shellHistoryArray *shell_history);
