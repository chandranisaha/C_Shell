#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/wait.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include "input_handler.h"
#include "activities.h"
#include "keyboard.h"
#include "fgbg.h"

#define MAX_ARGS 64

int backgrndpid=-1;

void execute_foreground(char *inputCommand, char *home)
{
    struct timeval start, end;

    char *command[MAX_ARGS]; // Array to hold command and arguments
    char *token;             // For tokenizing the input command
    int i = 0;

    // Tokenizing the input command string
    token = strtok(inputCommand, " ");
    while (token != NULL && i < MAX_ARGS - 1)
    {
        command[i++] = token; // Populate command array
        token = strtok(NULL, " ");
    }
    command[i] = NULL; // Null-terminate the array of strings

    if (strcmp(command[0], "cd") == 0)
    {
        if (command[1] == NULL)
        {
            // Change to home directory if no argument is provided
            chdir(home);
        }
        else
        {
            // Change to the specified directory
            if (chdir(command[1]) != 0)
            {
                perror("cd");
            }
        }
        return;
    }
    // Fork a new process

    gettimeofday(&start, NULL);

    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process: Execute the command

        if (execvp(command[0], command) == -1)
        {
            perror("Error executing command");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid > 0)
    {
        // Parent process: Wait for the child to complete
        int status;
        running_forground = pid;
        add_process(pid,inputCommand);
        add_process_fgbg(pid,inputCommand,1);

        waitpid(pid, &status, WUNTRACED);
        running_forground =0;
        gettimeofday(&end, NULL);
        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        time1 = elapsed_time;
        // printf("%d\n",time1);
    }
    else
    {
        // Fork failed
        perror("Failed to fork");
        // printf("here\n");

        exit(EXIT_FAILURE);
    }
    return;
}

void signal_handler(int sig)
{
    int status;
    // printf("check\n");

    pid_t pid;
    while ((pid = waitpid(backgrndpid, &status, WNOHANG)) > 0)
    {
        char procName[256];                         // Assuming process name will not exceed 255 characters
        sprintf(procName, "/proc/%d/cmdline", pid); // Path to the cmdline file in proc
        FILE *cmdline = fopen(procName, "r");
        if (cmdline)
        {
            fgets(procName, sizeof(procName), cmdline); // Read the command line
            fclose(cmdline);
        }
        else
        {
            strcpy(procName, "Unknown"); // Fallback if file can't be read
        }

        if (WIFEXITED(status))
        {
            printf("Background process  (%d) ended normally.\n",  pid);
        }
        else
        {
            printf("Background process %s (%d) ended abnormally.\n", procName, pid);
        }
    }
    return;
}

void execute_background(char *inputCommand, char *home)
{
    // char *command[MAX_ARGS];

    
    
    char *command[MAX_ARGS] = {"ls", "-l", NULL};

    char *token;
    int i = 0;

    // // Change working directory if home is not NULL
    // if (home != NULL) {
    //     chdir(home);
    // }

    // Tokenizing the input command string
    token = strtok(inputCommand, " ");
    while (token != NULL && i < MAX_ARGS - 1)
    {
        command[i++] = token;
        token = strtok(NULL, " ");
    }
    i++;
    command[i] = NULL;
    // if (i > 0 && strcmp(command[i - 1], "&") == 0)
    // {
    //     command[--i] = NULL; // Remove '&' from the arguments
    //     command[--i] = NULL; // Remove '&' from the arguments

    // }
    pid_t pid = fork();
    // printf("check\n");
    if (pid == 0)
    {
        // printf("%d ",i);
        // printf("check1 %s\n", command[0]);
        // for (int j = 0; j < i; j++)
        // {
        //     printf("command[%d]: %s\n", j, command[j]);
        // }
        // i++;
        // Child
        
        if (execvp(command[0], command) == -1) ///// getting seg fault
        {
            // printf("check\n");

            perror("Error executing command");
            exit(EXIT_FAILURE);
        }
        // printf("check5\n");
    }
    else if (pid > 0)
    {
        // Parent
        add_process(pid,inputCommand);
        add_process_fgbg(pid,inputCommand,0);
        printf("Background PID: %d\n", pid);
        signal(SIGCHLD, signal_handler);
        // printf("check2\n");
    }
    else
    {
        // printf("check3\n");

        // Fork fail
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    }
    return;

}
