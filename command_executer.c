#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include "command_executer.h"
#include "activities.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "system_command.h"
#include "proclore.h"
#include "seek.h"
#include "iman.h"
#include "iman.h"
#include "ping.h"
#include "neonate.h"
#include <wait.h>
#include "fgbg.h"

void execute_command(char *command, char *home, char *last)
{
    // printf("%s\n hello",command);
    // Remove leading and trailing spaces/tabs
    while (*command == ' ' || *command == '\t')
    {
        command++;
    }
    char *end = command + strlen(command) - 1;
    while (end > command && (*end == ' ' || *end == '\t'))
    {
        end--;
    }
    *(end + 1) = '\0';
    if (strlen(command) == 0)
    {
        return;
    }
    if (command[0] == 'a' && command[1] == 'c' && command[2] == 't' && command[3] == 'i' && command[4] == 'v' && command[5] == 'i' && command[6] == 't' && command[7] == 'i' && command[8] == 'e' && command[9] == 's' && (command[10] == ' ' || command[10] == '\0'))

    {
        // display_;
        print_processes();
    }
    // pid_t pid = fork();
    // if (strcmp(command, "activities") == 0)
    // {
    // }
    // else
    // {
    //     if (pid < 0)
    //     {
    //         // Error in fork
    //         perror("Fork failed");
    //     }
    //     else if (pid == 0)
    //     {
    //         // printf("hi1\n");
    //         // Child process code
    //         // Execute command here using exec()
    //     }
    //     else
    //     {
    //         // printf("hi  2\n");

    //         // Parent process
    //         add_process( pid,command);
    //     }
    // }
    // if (pid == 0)
    // {
    // Child
    // char *args[] = {"/bin/sh", "-c", command, NULL};
    // execvp(args[0], args);
    // perror("execvp failed");
    // exit(1);
    else if (command[0] == 'h' && command[1] == 'o' && command[2] == 'p' && (command[3] == ' ' || command[3] == '\0'))
    {
        hop(command, home, last);
    }
    else if (command[0] == 'r' && command[1] == 'e' && command[2] == 'v' && command[3] == 'e' && command[4] == 'a' && command[5] == 'l' && (command[6] == ' ' || command[6] == '\0'))

    {
        reveal(command, home, last);
    }
    else if (command[0] == 'l' && command[1] == 'o' && command[2] == 'g' && command[3] == ' ' && command[4] == 'p' && command[5] == 'u' && command[6] == 'r' && command[7] == 'g' && command[8] == 'e' && (command[9] == ' ' || command[9] == '\0'))
    {
        clear_history();
    }
    else if (command[0] == 'l' && command[1] == 'o' && command[2] == 'g' && command[3] == ' ' && command[4] == 'e' && command[5] == 'x' && command[6] == 'e' && command[7] == 'c' && command[8] == 'u' && command[9] == 't' && command[10] == 'e' && command[11] == ' ')
    {
        run_history(command, home, last);
    }
    else if (command[0] == 'l' && command[1] == 'o' && command[2] == 'g' && (command[3] == ' ' || command[3] == '\0'))
    {
        print_history();
    }
    else if (command[0] == 'p' && command[1] == 'r' && command[2] == 'o' && command[3] == 'c' && command[4] == 'l' && command[5] == 'o' && command[6] == 'r' && command[7] == 'e' && (command[8] == ' ' || command[8] == '\0'))

    {
        proc(command);
    }

    else if (command[0] == 's' && command[1] == 'e' && command[2] == 'e' && command[3] == 'k' && (command[4] == ' ' || command[4] == '\0'))
    {
        // printf("hehehe\n");
        seek_command(command, home, last);
    }
    else if (command[0] == 'i' && command[1] == 'M' && command[2] == 'a' && command[3] == 'n' && (command[4] == ' ' || command[4] == '\0'))
    {
       
        fetch_man_page(command);
    }
    else if (command[0] == 'f' && command[1] == 'g' && (command[2] == ' ' || command[2] == '\0'))
    {
        command += 2;

        while (*command == ' ' || *command == '\t')
        {
            command++;
        }

        // Convert the remaining string to an integer
        int num = atoi(command);

        pid_t pro = (pid_t)num;
        fg_and(pro);
        // printf("ok\n");
        // fetch_man_page(command);
    }
    else if (command[0] == 'b' && command[1] == 'g' && (command[2] == ' ' || command[2] == '\0'))
    {
        command += 2;

        while (*command == ' ' || *command == '\t')
        {
            command++;
        }

        // Convert the remaining string to an integer
        int num = atoi(command);

        pid_t pro = (pid_t)num;
        bg_and(pro);
        // fetch_man_page(command);
    }
    else if (command[0] == 'n' && command[1] == 'e' && command[2] == 'o' && command[3] == 'n' && command[4] == 'a' &&
             command[5] == 't' && command[6] == 'e' && command[7] == ' ' && command[8] == '-' && command[9] == 'n' &&
             (command[10] == ' ' || command[10] == '\0'))
    {
        command = command + 10;
        while (*command == ' ')
        {
            command++;
        }
        float time = atof(command);
        if(((int)(time*10))%10!=0)
        {
            printf("fraction passed\n");
            return;
        }
        // printf("%d",time);
        // signal(SIGINT, SIG_IGN);  // Ignore Ctrl+C
        // signal(SIGTSTP, SIG_IGN); // Ignore Ctrl+Z
        signal(SIGINT, stop_on_x);
        // Run the neonate command
        neonate(time);
    }
    else if (command[0] == 'n' && command[1] == 'e' && command[2] == 'o' && command[3] == 'n' && command[4] == 'a' &&
             command[5] == 't' && command[6] == 'e' && command[7] == ' ' && command[8] == '-')
    {
        printf("Error with the flag or argument missing\n");
        return;
    }
    else if (command[0] == 'p' && command[1] == 'i' && command[2] == 'n' && command[3] == 'g' && (command[4] == ' ' || command[4] == '\0'))
    {
        command += 4;
        ping_process(command);
        // printf("hehehe\n");
    }
    else
    {
        if (*end == '&')
        {
            // printf("%c\n", *end);Error executing command: No such file or directory
            // printf("%s\n", command);

            *end = '\0';
            // printf("%s\n", command);
            execute_background(command, home);
        }
        else
        {
            // printf("herede\n");

            execute_foreground(command, home);
        }
        // perror("invalid command");
    }
    // }
    // else if (pid > 0)
    // {
    //     // Parent
    //     add_process(pid, command);
    //     wait(NULL);
    // }
    // else
    // {
    //     // printf("here\n");
    //     perror("fork failed");
    // }
    return;
}
