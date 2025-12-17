#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include "fgbg.h"
#include "activities.h"
#include "keyboard.h"



process_t_fb processes_fb[100]; // Array to store the processes_fb
int process_count_fb = 0;    // Number of processes_fb being tracked

void update_process_state_fgbg()
{
    for (int i = 0; i < process_count_fb; i++)
    {

        ////////////////////////////////////
        // printf("ok\n");
        char proc_file[256];
        snprintf(proc_file, sizeof(proc_file), "/proc/%d/stat", processes_fb[i].pid);

    // Open the /proc/[pid]/stat file
        FILE *file = fopen(proc_file, "r");
        if (file == NULL) {
            // If the file can't be opened, the process doesn't exist
            processes_fb[i].is_running=-1;
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
        int pid = processes_fb[i].pid;       
        // printf("ok\n");

        // printf("ok\n");

        switch (state) {
            case 'R':
                processes_fb[i].is_running=1;
                break;
            case 'S':
                processes_fb[i].is_running=1;
                // printf("Process %d is sleeping\n", pid);
                break;
            case 'D':
                // processes[i].is_running=1;
                processes_fb[i].is_running=0;

                // printf("Process %d is in uninterruptible sleep\n", pid);
                break;
            case 'T':
                processes_fb[i].is_running=0;
                break;
            case 'Z':
                processes_fb[i].is_running=0;

                break;
            default:
                // printf("Process %d is in an unknown state: %c\n", pid, state);
                processes_fb[i].is_running=0;
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

void fg_and(pid_t pid)
{
    update_process_state_fgbg();
    int found = 0;
    for (int i = 0; i < process_count_fb; i++)
    {
        if (processes_fb[i].pid == pid)
        {
            found = 1;

            // Bring the process to the foreground
            processes_fb[i].is_foreground = 1;

            // Give terminal control to the process group
            // tcsetpgrp(STDIN_FILENO, pid);

            // Resume the process if it was stopped
            if (!processes_fb[i].is_running)
            {
                kill(pid, SIGCONT);          // Send SIGCONT to continue
                processes_fb[i].is_running = 1; // Mark it as running
            }

            // Wait for the process to finish or stop again
            int status;
            
            running_forground =pid;
            // printf("%d\n",pid);
            waitpid(pid, &status, WUNTRACED);
            running_forground =0;

            // If the process was stopped again
            if (WIFSTOPPED(status))
            {
                processes_fb[i].is_running = 0;
                printf("Process %d stopped\n", pid);
            }

            // Return terminal control to the shell
            // tcsetpgrp(STDIN_FILENO, getpid());
            break;
        }
    }

    if (!found)
    {
        printf("No such process found\n");
    }
}
void bg_and(pid_t pid)
{
    update_process_state_fgbg();
    int found = 0;
    for (int i = 0; i < process_count_fb; i++)
    {
        if (processes_fb[i].pid == pid)
        {
            found = 1;

            // Resume the process in the background
            
            if (!processes_fb[i].is_running)
            {
                kill(pid, SIGCONT);             // Send SIGCONT to continue
                processes_fb[i].is_running = 1;    // Mark it as running
                processes_fb[i].is_foreground = 0; // It's now running in the background
                printf("Process %d is now running in the background\n", pid);
            }
            else
            {
                printf("Process %d is already running\n", pid);
            }
            break;
        }
    }

    if (!found)
    {
        printf("No such process found\n");
    }
}
void add_process_fgbg(pid_t pid, char *command, int is_foreground)
{
    process_t_fb new_process;
    new_process.pid = pid;
    strcpy(new_process.command, command);
    new_process.is_running = 1; // Mark as running initially
    new_process.is_foreground = is_foreground;

    processes_fb[process_count_fb++] = new_process;
}
