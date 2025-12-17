#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "common.h"
#include "log.h"
#define MAX_ENTRIES 15
#define HISTORY_FILE "/home/pratishtha/Desktop/sem5/OSN/mp1repo/mini-project-1-Pratishtha3Saxena/history.txt"
#include "command_executer.h"

int cmp2(const char *a, const char *b)
{
    int len = (strlen(a) > strlen(b)) ? strlen(b) : strlen(a);

    for (int i = 0; i < len; i++)
    {
        if (a[i] != b[i])
            return 1;
    }
    return 0;
}

// int store_history(char *input, int count)
void print_history()
{
    logon = 1;
    FILE *file;
    char buffer[4096];

    // Open the file in read mode
    file = fopen(HISTORY_FILE, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Read and print each line in the file

    char *array[15];
    int cnt = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        array[cnt] = malloc(sizeof(char) * 4096);
        strcpy(array[cnt], buffer);
        cnt++;
    }

    for (int i = cnt - 1; i >= 0; i--)
    {
        printf("%s", array[i]);
    }

    // Close the file
    fclose(file);
}

void run_history(char *command, char *home, char *last)
{
    logon = 1;
    FILE *file;
    char buffer[4096];

    // Open the file in read mode
    file = fopen(HISTORY_FILE, "r");
    if (file == NULL)
    {
        printf("here\n");

        perror("Error opening file");
        return;
    }

    // Read and print each line in the file

    char *array[15];
    int cnt = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        array[cnt] = malloc(sizeof(char) * 4096);
        strcpy(array[cnt], buffer);
        cnt++;
    }

    command += 11;
    while (*command == ' ' || *command == '\t')
    {
        command++;
    }

    // printf("%s\n",command);
    int no = atoi(command);
    array[cnt - no][strlen(array[cnt - no]) - 1] = '\0';
    store_history(array[cnt - no]);
    execute_command(array[cnt - no], home, last);
}

void clear_history()
{
    logon = 1;
    FILE *file;
    char buffer[4096];

    // Open the file in read mode
    file = fopen(HISTORY_FILE, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    fclose(file);
}
void store_history(char *str)
{
    // printf("hello\n");
    char *input = str;
    FILE *file;
    char *lines[MAX_ENTRIES];
    char buffer[4096];
    int i = 0;

    // Open the file in read mode to read existing entries
    file = fopen(HISTORY_FILE, "r");
    if (file != NULL)
    {
        // printf("hello\n");

        // printf("hello\n");

        while (fgets(buffer, sizeof(buffer), file))
        {
            lines[i] = strdup(buffer);
            char end[3];
            end[0] = '\n';
            // printf("line:%d\n", i);
            // printf("line: %s", lines[i]);

            if (i >= MAX_ENTRIES || strcmp(lines[i], end) == 0)
            {

                break; // In case there are more than MAX_ENTRIES, we don't need to read further
            }
            i++;
        }
        fclose(file);
    }
    else
    {
        // printf("hello\n");

        perror("error\n");
        return;
    }
    // printf("hello\n");

    // Check if the new input is exactly the same as the last input
    int cmp1 = cmp2(lines[i - 1], input);
    // printf("first %s ", lines[i-1]);
    // printf("%s and %s and %d\n",input,lines[i-1],cmp1);
    // printf("second %s ", input);

    // printf("%d\n", cmp);
    if (i > 0 && cmp1 == 0)
    {
        // If the input matches the last entry, do nothing and return the current count
        for (int j = 0; j < i; j++)
        {
            free(lines[j]);
        }
        return;
        // return count;
    }

    // If there are already MAX_ENTRIES, remove the oldest entry (lines[0])
    if (i >= MAX_ENTRIES)
    {
        free(lines[0]);
        for (int j = 1; j < MAX_ENTRIES; j++)
        {
            lines[j - 1] = lines[j];
        }
        i--; // Decrement i to reflect the removal of the oldest entry
    }

    // Add the new input as the last entry

    lines[i] = strdup(input);
    // printf("%s %d\n",lines[i],i);
    i++;

    // printf("%d\n", i);
    // Open the file in write mode to save the updated entries
    file = fopen(HISTORY_FILE, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        // return count;
    }
    for (int j = 0; j < i; j++)
    {
        fprintf(file, "%s", lines[j]);
        free(lines[j]); // Free memory after writing to the file
    }
    char end[3];
    end[0] = '\n';
    fprintf(file, "%s", end);
    fclose(file);
    // Update the count
    // count++;
    // return count;
}
