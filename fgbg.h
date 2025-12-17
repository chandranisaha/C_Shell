#ifndef _HOP_H_
#define _HOP_H_
typedef struct process_fb
{
    pid_t pid;
    char command[256];
    int is_running;    // 1 = running, 0 = stopped
    int is_foreground; // 1 = foreground, 0 = background
} process_t_fb;
void fg_and(pid_t pid);
void bg_and(pid_t pid);
extern int process_count_fb;    // Number of processes_fb being tracked

void add_process_fgbg(pid_t pid, char *command, int is_foreground);
extern process_t_fb processes_fb[100]; // Array to store the processes_fb

#endif