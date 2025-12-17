#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ping.h"
#include <sys/types.h>
// Function to send a signal to a process
void ping_process(char *command)
{
    // pid_t pid, int signal_number;
    // int signal = signal_number % 32;

    char *pid_str = strtok(command, " ");
    if (pid_str == NULL)
    {
        fprintf(stderr, "Error: Invalid input, couldn't extract PID.\n");
        return;
    }

    // Extract the second token as sig
    char *sig_str = strtok(NULL, " ");
    if (sig_str == NULL)
    {
        fprintf(stderr, "Error: Invalid input, couldn't extract signal.\n");
        return;
    }

    // Convert pid and sig to integers (optional, depending on what you need)
    pid_t pid = atoi(pid_str);
    int sigg = atoi(sig_str);
    sigg = sigg % 32;
    // Send the signal to the process
    if (kill(pid, sigg) == -1)
    {
        if (errno == ESRCH)
        {
            // No such process exists
            printf("No such process found\n");
        }
        else
        {
            perror("Error sending signal");
        }
    }
    else
    {
        printf("Signal %d sent to process %d\n", sigg, pid);
    }
}
