#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "piping.h"
#include "input_handler.h"
#define MAX_COMMANDS 100
char *trim_whitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator
    *(end + 1) = 0;

    return str;
}
void check_pipe(char *input, char *home, char *last)
{
    char *commands[MAX_COMMANDS]; // Store individual commands after split by pipe
    int num_commands = 0;
    int foundAmpersand = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '&')
        {
            foundAmpersand = 1; // Mark that & was found
        }
        if (input[i] == '|')
        {
            if (foundAmpersand)
            {
                fprintf(stderr, "Invalid use of pipe with &\n");

                // printf("Error: '&' appears before '|'\n");
                return; // Exit with error status
            }
        }
    }
    // Split the input by the pipe symbol '|'
    char *token = strtok(input, "|");
    while (token != NULL)
    {
        commands[num_commands++] = trim_whitespace(token); // Trim each token
        token = strtok(NULL, "|");
    }

    // Validate pipe usage: no empty command before/after pipe
    for (int i = 0; i < num_commands; i++)
    {
        if (strcmp(commands[i], "") == 0)
        {
            fprintf(stderr, "Invalid use of pipe\n");
            return;
        }
    }

    int pipefds[2 * (num_commands - 1)]; // Array to hold pipe file descriptors

    // Create pipes for each command
    for (int i = 0; i < num_commands - 1; i++)
    {
        if (pipe(pipefds + i * 2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    int status;
    if (num_commands <= 1)
    {
        // printf("%s\n",commands[0]);
        handle_bgANDfile(commands[0], home, last);
        // printf("ee\n");
        return;
    }
    pid_t pid;

    for (int i = 0; i < num_commands; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) // Child process
        {
            // Redirect input for commands after the first one
            if (i > 0)
            {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) == -1)
                {
                    perror("dup2 input");
                    exit(EXIT_FAILURE);
                }
            }

            // Redirect output for commands before the last one
            if (i < num_commands - 1)
            {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 output");
                    exit(EXIT_FAILURE);
                }
            }

            // Close all pipe file descriptors (important in child process)
            for (int j = 0; j < 2 * (num_commands - 1); j++)
            {
                close(pipefds[j]);
            }

            // Execute the command (replace with your execution function)
            handle_bgANDfile(commands[i], home, last);
            exit(0); // Exit child after command execution
        }
    }

    // Parent process: close all pipe file descriptors
    for (int i = 0; i < 2 * (num_commands - 1); i++)
    {
        close(pipefds[i]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_commands; i++)
    {
        wait(&status);
    }

    return;
}

// {
//     // char
//     int cols = sizeof(input);
//     int *array = (int **)malloc(1 + sizeof(int *)); // Array of pocharers
//     for (int j = 0; j < cols; j++)
//     {
//         array[j] = -1; // Initialize each element to -1
//     }
//     //  ____________________________________________________
//     int count = 0;
//     int open = 0;
//     int close = 0;
//     for (int i = 0; i < cols; i++)
//     {
//         if (input[i] == '|' && open == 0)
//         {
//             array[count + 1] = i;
//             open = 1;
//             count++;
//         }
//         else if (input[i] == '|' && open == 1)
//         {
//             // array[1][count - 1] = i;
//             // array[0][count] = i;
//             array[count + 1] = i;
//             open = 0;
//         }
//     }
//     array[0] = count;
//     return array;
//     pid_t pid = fork();
//     if (pid == -1)
//     {
//         perror("fork failed");
//         exit(1);
//     }

//     if (pid == 0)
//     {
//         // Child process
//         if (i > 0)
//         {
//             dup2(prev_fd, STDIN_FILENO); // Get input from the previous pipe
//             close(prev_fd);
//         }
//         if (i < pipe_count - 1)
//         {
//             dup2(pipes[1], STDOUT_FILENO); // Send output to the next pipe
//             close(pipes[0]);
//             close(pipes[1]);
//         }

//         // Execute the current command
//         execute_command(commands[i]);
//     }
//     else
//     {
//         // Parent process
//         wait(NULL); // Wait for the child to finish
//         if (i > 0)
//         {
//             close(prev_fd); // Close the previous read end
//         }
//         if (i < pipe_count - 1)
//         {
//             close(pipes[1]);    // Close the write end of the current pipe
//             prev_fd = pipes[0]; // Save the read end for the next command
//         }
//     }
//     //  ____________________________________________________

// }