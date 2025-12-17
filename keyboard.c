#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
pid_t foreground_pid = -1; // Track the foreground process (if any)
int running_forground;
#include "keyboard.h"
#include "activities.h"

// Handler for Ctrl+C (SIGINT)
void handle_sigint(int signum)
{
    if (running_forground != 0)
    {
        // printf("\nCaught Ctrl+C (SIGINT), performing custom action...\n");
        if (running_forground == 0)
        {

            kill(foreground_pid,19);
            printf("%d\n", running_forground);
        }

        // printf("Custom action complete. Resuming program...\n");
    }
    else
    {
        // printf("\nCtrl+C pressed but the flag is off. Ignoring...\n");
    }
}

// Handler for Ctrl+Z (SIGTSTP)
void handle_sigtstp(int sig)
{
    if (running_forground != 0)
    {
        // printf("\nCaught Ctrl+C (SIGINT), performing custom action...\n");

        // printf("%d\n", running_forground);
        if (kill(running_forground, 20) == -1)
        {
            if (errno == ESRCH)
            {
                printf("No such process found\n");
            }
            else
            {
                // perror("Error sending signal");
            }
        }
        else
        {
            printf("Signal %d sent to process %d\n", 20, running_forground);
        }
        running_forground = 0;
        return;
        // printf("Process %d stopped\n", foreground_pid);
        // Here, you could move it to background or stop further actions
    }
    else if (running_forground == 0)
    {
        // fork();

        // printf("No foreground process to stop\n");
        // sigjmp(env, 1);
        return;
    }
}

// Function to log out (Ctrl+D behavior)
void logout_shell()
{
    printf("Logging out... Killing all processes.\n");

    for (int i=0;i<process_count;i++)
    {
        int ok = processes[i].pid;
        kill(ok,9);
    }

    // for (int i = 0; i < process_count; i++)
    // {
    //     if (processes[i].is_running)
    //     {
    //         kill(processes[i].pid, SIGTERM); // Send SIGTERM to all processes
    //     }
    // }
    exit(0);
}
