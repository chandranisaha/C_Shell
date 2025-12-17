#ifndef SYSTEM_COMMAND
#define SYSTEM_COMMAND
#define MAX_ARGS 64

void execute_foreground(char *inputCommand,char*home);
void execute_background(char *command,char* home);
void signal_handler(int sig);

#endif // SYSTEM_COMMAND
