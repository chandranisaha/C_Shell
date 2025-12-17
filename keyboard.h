#ifndef PING_H
#define PING_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

extern sigjmp_buf env;

// void hop(char *adress,char *home,char*last);
void handle_sigint(int sig);
void handle_sigtstp(int sig);
void logout_shell();

extern  int running_forground;
extern pid_t foreground_pid ;
#endif // PING_H
