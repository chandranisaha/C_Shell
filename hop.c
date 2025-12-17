#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "hop.h"

void hop2(char *adress, char *home, char *last)
{
    // printf("ok\n");
    char *directory = adress;
    if (*directory == '~')
    {
        chdir(home);
        printf("Changed directory to: %s\n", home);
    }
    else if (*directory == '-')
    {
        // printf("ok\n");
        // printf("%s\n",last);
        chdir(last);
        printf("Changed directory to: %s\n", last);
    }
    else if (chdir(directory) == 0)
    {
        // chdir(adress);
        // printf("ok\n");
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("Changed directory to: %s\n", cwd);
        }
        else
        {
            perror("getcwd() error");
        }
    }
    else
    {
        // printf("hello\n");
        perror("chdir() failed");
    }
    return;
}

void hop(char *adress, char *home, char *last)
{
    adress = adress + 3;
    while (*adress == ' ' || *adress == '\t')
    {
        adress++;
    }
    char *directory = adress;

    char *token;
    char *rest = directory;
    // int count=0;
    while ((token = strtok_r(rest, " ", &rest)))
    {
        // // printf("%s", token);
        // printf("%d\n",count);
        // count++;
        hop2(token, home, last);
    }
    // chdir()
}
