#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include <sys/types.h>

// Define the process structure
typedef struct process
{
    pid_t pid;           // Process ID
    char command[256];   // Command name
    int is_running;      // 1 = running, 0 = stopped
} process_t;

// Define an array to store processes and track the process count
extern process_t processes[100]; // Array to store processes
extern int process_count;        // Number of processes being tracked

// Function declarations
void add_process(pid_t pid, char *command);
void update_process_state();
void sort_processes();
void print_processes();

#endif // ACTIVITIES_H
