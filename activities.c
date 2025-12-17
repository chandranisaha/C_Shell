// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/types.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <errno.h>

// typedef struct process
// {
//     pid_t pid;
//     char command[256];
//     int is_running; // 1 = running, 0 = stopped
// } process_t;

// process_t processes[100]; // Array to store the processes
// int process_count = 0;    // Number of processes being tracked

// // Function to add a process to the list
// void add_process(pid_t pid, char *command)
// {
//     process_t new_process;
//     new_process.pid = pid;
//     strcpy(new_process.command, command);
//     new_process.is_running = 1; // Initially, the process is running
//     processes[process_count++] = new_process;
// }

// // Function to update the state of each process using /proc/[pid]/stat
// void update_process_state()
// {
//     for (int i = 0; i < process_count; i++)
//     {
//         char proc_stat_path[256];
//         snprintf(proc_stat_path, sizeof(proc_stat_path), "/proc/%d/stat", processes[i].pid);

//         FILE *stat_file = fopen(proc_stat_path, "r");
//         if (stat_file == NULL)
//         {
//             // If the file can't be opened, the process might have terminated or the PID might be invalid
//             if (errno == ENOENT)
//             {
//                 // Process has terminated (no such file or directory)
//                 processes[i].is_running = 0;
//             }
//             else
//             {
//                 // Handle other possible errors, just mark it as not running for now
//                 processes[i].is_running = 0;
//             }
//             continue;
//         }

//         // Read the process status from /proc/[pid]/stat
//         char buffer[1024];
//         if (fgets(buffer, sizeof(buffer), stat_file) != NULL)
//         {
//             // The 3rd field in /proc/[pid]/stat contains the state
//             char state;
//             sscanf(buffer, "%*d %*s %c", &state);

//             // 'R' for running, 'S' for sleeping, 'D' for uninterruptible sleep, 'T' for stopped
//             if (state == 'R' || state == 'S' || state == 'D')
//             {
//                 processes[i].is_running = 1; // Running
//             }
//             else if (state == 'T')
//             {
//                 processes[i].is_running = 0; // Stopped
//             }
//         }

//         fclose(stat_file);
//     }
// }

// // Function to sort processes by their PID (lexicographically)
// void sort_processes()
// {
//     for (int i = 0; i < process_count - 1; i++)
//     {
//         for (int j = i + 1; j < process_count; j++)
//         {
//             if (processes[i].pid > processes[j].pid)
//             {
//                 process_t temp = processes[i];
//                 processes[i] = processes[j];
//                 processes[j] = temp;
//             }
//         }
//     }
// }

// // Function to print the list of processes
// void print_processes()
// {
//     update_process_state(); // Update the process states before printing
//     sort_processes();       // Sort processes by PID

//     for (int i = 0; i < process_count; i++)
//     {
//         printf("%d : %s - %s\n", processes[i].pid, processes[i].command,
//                processes[i].is_running ? "Running" : "Stopped");
//     }
// }

// // Example main function for testing
// // int main()
// // {
// //     // Simulate adding a few processes (this happens on fork in a real shell)
// //     add_process(221, "emacs new.txt");
// //     add_process(430, "vim");
// //     add_process(620, "gedit");

// //     // Simulate the 'activities' command to print processes
// //     printf("<Shell@SYS:~> activities\n");
// //     print_processes(); // Print the process list

// //     return 0;
// // }
// ______________________________________________________________________________________
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "activities.h"

// typedef struct process
// {
//     pid_t pid;
//     char command[256];
//     int is_running;
// } process_t;

process_t processes[100]; // Array to store the processes
int process_count = 0;    // Number of processes being tracked

// Function to add a process to the list
void add_process(pid_t pid, char *command)
{

    processes[process_count].pid = pid;
    processes[process_count].is_running = 1;
    strcpy(processes[process_count].command, command);
    process_count += 1;
    // printf("DEBUG AP: %d\n", process_count);
    return;
}

// Function to update the state of each process (running or stopped)
void update_process_state()
{
    for (int i = 0; i < process_count; i++)
    {

        ////////////////////////////////////
        // printf("ok\n");
        char proc_file[256];
        snprintf(proc_file, sizeof(proc_file), "/proc/%d/stat", processes[i].pid);

    // Open the /proc/[pid]/stat file
        FILE *file = fopen(proc_file, "r");
        if (file == NULL) {
            // If the file can't be opened, the process doesn't exist
            processes[i].is_running=-1;
            continue;
        }
        // printf("ok\n");


    // Read the content of the file
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
        // The third field in /proc/[pid]/stat is the process state
            char state;
            sscanf(buffer, "%*d %*s %c", &state);

        // Process state codes:
        // 'R' = Running, 'S' = Sleeping, 'D' = Uninterruptible Sleep, 'T' = Stopped, 'Z' = Zombie
        int pid = processes[i].pid;       
        // printf("ok\n");

        // printf("ok\n");

        switch (state) {
            case 'R':
                processes[i].is_running=1;
                break;
            case 'S':
                processes[i].is_running=1;
                // printf("Process %d is sleeping\n", pid);
                break;
            case 'D':
                // processes[i].is_running=1;
                processes[i].is_running=0;

                // printf("Process %d is in uninterruptible sleep\n", pid);
                break;
            case 'T':
                processes[i].is_running=0;
                break;
            case 'Z':
                processes[i].is_running=0;

                break;
            default:
                // printf("Process %d is in an unknown state: %c\n", pid, state);
                processes[i].is_running=0;
                break;
            }
        // printf("ok\n");

        }

    // Close the file
        // fclose(file);


        // int status;
        // printf("DEBUG  UP: %d\n", process_count);
        // pid_t result = waitpid(processes[i].pid, &status, WNOHANG);
        // if (result == 0)
        // {
        //     processes[i].is_running = 1; // Running
        // }
        // else if (WIFSTOPPED(status))
        // {
        //     processes[i].is_running = 0; // Stopped
        // }
    }
    return;
}

// Function to sort processes by their PID (lexicographically)
void sort_processes()
{
    // printf("DEBUG SP: %d\n", process_count);

    for (int i = 0; i < process_count - 1; i++)
    {
        for (int j = i + 1; j < process_count; j++)
        {
            if (processes[i].pid > processes[j].pid)
            {
                process_t temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
    return;
}

// Function to print the list of processes
void print_processes()
{
    update_process_state(); // Update the process states before printing
    sort_processes();       // Sort processes by PID
    // printf("DEBUG: %d\n", process_count);

    // Debug print to verify the number of processes

    for (int i = 0; i < process_count; i++)
    {
        // printf("%dokok\n",i);
        if(processes[i].is_running==-1)
        continue;
        printf("%d : %s - %s\n", processes[i].pid, processes[i].command,
               processes[i].is_running==1 ? "Running" : "Stopped");
    }
}
