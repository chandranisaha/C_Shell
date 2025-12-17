#ifndef LOG_H
#define LOG_H
void store_history(char *str);
void print_history();
void clear_history();
void run_history(char* command,char*home,char* last);

extern int logon;

#endif // LOG_H
