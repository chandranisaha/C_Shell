#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include "input_handler.h"
#include "activities.h"
#include "command_executer.h"
#include "seek.h"
#include "piping.h"
#include "aliasing.h"
// void ready(token, original_stdin, original_stdout, flag, home, last, append)
// {
// int in = 0;
// int out = 0;
// char *dummy = token;
// char *token2 = token;
// // printf("%d\n",count);
// // printf("helloooooo\n");
// // count++;
// // ________________________________________________________
// int j = 0;
// int k = 0;
// int jj = 0;
// int kk = 0;
// int exec = 0;
// // char destination[500];
// char *destination = malloc(sizeof(char) * 500);
// while (*dummy != '\0')
// {
//     if (*dummy == '&' && (*(dummy + 1) == '\0' || *(dummy + 1) == ' '))
//     {
//         // printf("helloooooo\n");
//         if (*(dummy + 1) == '\0')
//         {
//             flag = 1;
//         }
//         *(dummy + 1) = '\0';
//         // printf("rferf\n");
//         execute_command(token, home, last);
//         // printf("rferf\n");
//         exec = 1;
//         token = dummy + 2;
//         if (in)
//         {
//             dup2(original_stdin, STDIN_FILENO);
//             close(original_stdin);
//             in = 0;
//         }
//         if (out)
//         {
//             dup2(original_stdout, STDOUT_FILENO);
//             close(original_stdout);
//             out = 0;
//         }

//         // printf("%s\n",dummy);
//         // printf("%s\n",token);

//         // printf("helloooooo\n");
//         // token = dummy + 2;
//     }
//     // if (exec == 0 && (in == 1 || out == 1))
//     // {
//     //     execute_command(token, home, last);
//     // }
//     if (*dummy == '>')
//     {
//         memset(destination, 0, sizeof(destination));

//         *dummy = '\0';
//         (dummy)++;
//         if (*dummy == '>')
//         {
//             append = 1;
//             *dummy = '\0';
//             (dummy)++;
//         }
//         while (*dummy == ' ')
//         {
//             (dummy)++;
//         }
//         while (*dummy != ' ' && *dummy != '\0')
//         {
//             destination[k] = *dummy; // Copy character to destination
//             k++;
//             (dummy)++;
//         }

//         destination[k] = '\0';
//         int fd_out;
//         if (append)
//         {
//             fd_out = open(destination, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             append = 0;
//         }
//         else
//         {
//             fd_out = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         }
//         if (fd_out < 0)
//         {
//             perror("Error opening output file");
//             exit(1);
//         }
//         dup2(fd_out, STDOUT_FILENO); // Redirect stdout to the file
//         close(fd_out);
//         // execute_command(token2, home, last);
//         // flag = 1;
//         out = 1;
//         (dummy)--;
//     }
//     if (*dummy == '<')
//     {
//         // printf( "tets\n");
//         *dummy = '\0';

//         (dummy)++;
//         memset(destination, 0, sizeof(destination));
//         while (*dummy == ' ')
//         {
//             (dummy)++;
//         }
//         while (*dummy != ' ' && *dummy != '\0')
//         {
//             destination[kk] = *dummy; // Copy character to destination
//             kk++;
//             (dummy)++;
//         }

//         int fd_in = open(destination, O_RDONLY);
//         if (fd_in < 0)
//         {
//             perror("No such input file found!");
//             exit(1);
//         }
//         dup2(fd_in, STDIN_FILENO); // Redirect stdin to the file
//         close(fd_in);
//         in = 1;
//         // execute_command(token2, home, last);
//         // flag = 1;

//         (dummy)--;
//     }
//     // if (token[j] == '|')
//     // {

//     // }

//     (dummy)++;
//     // (dummy)++;

//     // printf("%c",*dummy);
// }
// // char *dummy = token;
// // while (*dummy != '\0' && *dummy != '\n')
// // {
// // printf("helloooooo\n");

// // if (*dummy == '&' && (*(dummy + 1) == '\0' || *(dummy + 1) == ' '))
// // {
// //     // printf("helloooooo\n");
// //     if (*(dummy + 1) == '\0')
// //     {
// //         flag = 1;
// //     }
// //     *(dummy + 1) = '\0';
// //     execute_command(token, home, last);
// //     if (in)
// //     {
// //         dup2(original_stdin, STDIN_FILENO);
// //         close(original_stdin);
// //         in = 0;
// //     }
// //     if (out)
// //     {
// //         dup2(original_stdout, STDOUT_FILENO);
// //         close(original_stdout);
// //         out = 0;
// //     }

// //     // printf("%s\n",dummy);
// //     // printf("%s\n",token);

// //     // printf("helloooooo\n");
// //     token = dummy + 2;
// // }
// //     dummy++;
// // }

// // printf("%s", token);
// if (flag == 0)
// {
//     // printf("helloooooo\n");
//     execute_command(token, home, last);
//     if (in)
//     {
//         // printf("helloooooo in\n");

//         dup2(original_stdin, STDIN_FILENO);
//         close(original_stdin);
//         in = 0;
//     }
//     if (out)
//     {
//         // printf("helloooooo out\n");

//         dup2(original_stdout, STDOUT_FILENO);
//         close(original_stdout);
//         out = 0;
//     }
// }
// }
void handle_input(char *input2, char *home, char *last)
{
    // printf("plp\n");
    char *token;
    char *token22;

    char *rest = input2;
    char *rest22 = malloc(sizeof(char)*4096);
    strcpy(rest22,input2);

    // int count=0;
    const char *delimiters = ";";
    // char *token;
    int countyy = 0;
    int countyy_inn = 0;

    int original_stdin = dup(STDIN_FILENO);
    while ((token22 = strtok_r(rest22, delimiters, &rest22)))
    {
        countyy++;
    }
    // printf("%d\n",countyy);
    int original_stdout = dup(STDOUT_FILENO);
    while ((token = strtok_r(rest, delimiters, &rest)) && countyy_inn<countyy)
    {

        // printf("%s\n",token);
        countyy_inn++;
        
        int len = strlen(token);
        int start = 0;
        int end = len - 1;

        // Trim leading spaces and tabs
        while (isspace(token[start]) && start < len)
        {
            start++;
        }

        // Trim trailing spaces and tabs
        while (isspace(token[end]) && end >= start)
        {
            end--;
        }

        // Check if first or last character (after trimming) is '|'
        if (token[start] == '|' || token[end] == '|')
        {
            perror("Invalid use of pipe");
            return;
        }

        check_pipe(token, home, last);
        // printf("%s\n",token);
    }
}
void handle_bgANDfile(char *token, char *home, char *last)
{
    char *alias_command = get_alias_command(token);

    if (alias_command)
    {
        // Ensure the token buffer is large enough for the alias_command
        // Assume token has a size of TOKEN_SIZE
        int TOKEN_SIZE = sizeof(alias_command);
        TOKEN_SIZE++;
        strncpy(token, alias_command, TOKEN_SIZE - 1); // Copy alias command safely
        token[TOKEN_SIZE - 1] = '\0';                  // Make sure it's null-terminated
    }

    // else
    // {
    //     printf("1\n");
    //     int function_body = execute_function(token,home,last);
    //     printf("2\n");

    //     if (function_body == 1)
    //     {
    //         return;
    //     }
    //     // system(function_body); // This is a simple way, but parsing is needed for real use
    // }
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    int flag = 0;
    int append = 0;
    int in = 0;
    int out = 0;
    char *dummy = token;
    char *token2 = token;
    // printf("%d\n",count);
    // printf("helloooooo\n");
    // count++;
    // ________________________________________________________
    int j = 0;
    int k = 0;
    int jj = 0;
    int kk = 0;
    int exec = 0;
    // char destination[500];
    char *destination = malloc(sizeof(char) * 500);
    while (*dummy != '\0')
    {
        if (*dummy == '&' && (*(dummy + 1) == '\0' || *(dummy + 1) == ' '))
        {
            // printf("helloooooo\n");
            if (*(dummy + 1) == '\0')
            {
                flag = 1;
            }
            *(dummy + 1) = '\0';
            // printf("rferf\n");
            execute_command(token, home, last);
            // printf("rferf\n");
            exec = 1;
            token = dummy + 2;
            dummy++;
            if (in)
            {
                dup2(original_stdin, STDIN_FILENO);
                close(original_stdin);
                in = 0;
            }
            if (out)
            {
                dup2(original_stdout, STDOUT_FILENO);
                close(original_stdout);
                out = 0;
            }

            // printf("%s\n",dummy);
            // printf("%s\n",token);

            // printf("helloooooo\n");
            // token = dummy + 2;
        }
        // if (exec == 0 && (in == 1 || out == 1))
        // {
        //     execute_command(token, home, last);
        // }
        if (*dummy == '>')
        {
            memset(destination, 0, sizeof(destination));

            *dummy = '\0';
            (dummy)++;
            if (*dummy == '>')
            {
                append = 1;
                *dummy = '\0';
                (dummy)++;
            }
            while (*dummy == ' ')
            {
                (dummy)++;
            }
            while (*dummy != ' ' && *dummy != '\0')
            {
                destination[k] = *dummy; // Copy character to destination
                k++;
                (dummy)++;
            }

            destination[k] = '\0';
            int fd_out;
            if (append)
            {
                fd_out = open(destination, O_WRONLY | O_CREAT | O_APPEND, 0644);
                append = 0;
            }
            else
            {
                fd_out = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (fd_out < 0)
            {
                perror("Error opening output file");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO); // Redirect stdout to the file
            close(fd_out);
            // execute_command(token2, home, last);
            // flag = 1;
            out = 1;
            (dummy)--;
        }
        if (*dummy == '<')
        {
            // printf( "tets\n");
            *dummy = '\0';

            (dummy)++;
            memset(destination, 0, sizeof(destination));
            while (*dummy == ' ')
            {
                (dummy)++;
            }
            while (*dummy != ' ' && *dummy != '\0')
            {
                destination[kk] = *dummy; // Copy character to destination
                kk++;
                (dummy)++;
            }

            int fd_in = open(destination, O_RDONLY);
            if (fd_in < 0)
            {
                perror("No such input file found!");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO); // Redirect stdin to the file
            close(fd_in);
            in = 1;
            // execute_command(token2, home, last);
            // flag = 1;

            (dummy)--;
        }
        // if (token[j] == '|')
        // {

        // }

        (dummy)++;
        // (dummy)++;

        // printf("%c",*dummy);
    }
    // char *dummy = token;
    // while (*dummy != '\0' && *dummy != '\n')
    // {
    // printf("helloooooo\n");

    // if (*dummy == '&' && (*(dummy + 1) == '\0' || *(dummy + 1) == ' '))
    // {
    //     // printf("helloooooo\n");
    //     if (*(dummy + 1) == '\0')
    //     {
    //         flag = 1;
    //     }
    //     *(dummy + 1) = '\0';
    //     execute_command(token, home, last);
    //     if (in)
    //     {
    //         dup2(original_stdin, STDIN_FILENO);
    //         close(original_stdin);
    //         in = 0;
    //     }
    //     if (out)
    //     {
    //         dup2(original_stdout, STDOUT_FILENO);
    //         close(original_stdout);
    //         out = 0;
    //     }

    //     // printf("%s\n",dummy);
    //     // printf("%s\n",token);

    //     // printf("helloooooo\n");
    //     token = dummy + 2;
    // }
    //     dummy++;
    // }

    // printf("%s", token);
    if (flag == 0)
    {
        // printf("helloooooo\n");
        // printf("%s\n 1d", token);

        execute_command(token, home, last);
        // printf("%s\n 2nd\n hhh\n", token);
        if (in)
        {
            // printf("helloooooo in\n");

            dup2(original_stdin, STDIN_FILENO);
            close(original_stdin);
            in = 0;
        }
        if (out)
        {
            // printf("helloooooo out\n");

            dup2(original_stdout, STDOUT_FILENO);
            close(original_stdout);
            out = 0;
        }
    }
    // printf("heifhiwe\n");
    return;
}
// printf("%s\n %dih\n",maimarungajump,maj);
