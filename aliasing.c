#include "aliasing.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command_executer.h"
Alias aliases[MAX_ALIASES]; // Array to store aliases
int alias_count = 0;        // Initialize alias count to 0

Function functions[MAX_FUNCTIONS]; // Array to store functions
int function_count = 0;            // Initialize function count to 0

// Function to load aliases and functions from the .myshrc file
void load_myshrc()
{
    FILE *file = fopen(".myshrc", "r");
    if (file == NULL)
    {
        printf("No .myshrc file found.\n");
        return;
    }

    char line[256];
    int inside_function = 0;
    char func_name[50];
    char func_body[500];

    while (fgets(line, sizeof(line), file))
    {
        // Remove newline at the end
        line[strcspn(line, "\n")] = 0;

        // Parse alias definitions
        if (strncmp(line, "alias", 5) == 0)
        {
            char *alias_name = strtok(line + 6, " =");
            char *alias_command = strtok(NULL, "=");
            if (alias_name && alias_command)
            {
                strcpy(aliases[alias_count].alias, alias_name);
                strcpy(aliases[alias_count].command, alias_command);
                alias_count++;
            }
        }
        // Parse function definitions
        else if (strncmp(line, "func", 4) == 0)
        {
            inside_function = 1;
            sscanf(line + 5, "%s", func_name); // Get the function name
            strcpy(func_body, "");             // Reset function body
        }
        else if (inside_function && strchr(line, '}'))
        {
            // End of function definition
            inside_function = 0;
            strcpy(functions[function_count].func_name, func_name);
            strcpy(functions[function_count].func_body, func_body);
            function_count++;
        }
        else if (inside_function)
        {
            // Collect function body
            strcat(func_body, line);
            strcat(func_body, "\n");
        }
    }

    fclose(file);
}

// Function to get the command for a given alias
char *get_alias_command(char *command)
{
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(command, aliases[i].alias) == 0)
        {
            return aliases[i].command;
        }
    }
    return NULL; // Return NULL if no alias found
}

// // Function to get the function body for a given function name
// char* get_function_command(char *func_name) {
//     for (int i = 0; i < function_count; i++) {
//         if (strcmp(func_name, functions[i].func_name) == 0) {
//             return functions[i].func_body;
//         }
//     }
//     return NULL;  // Return NULL if no function found
// }

void parse_command(char *command, char **args)
{
    char *token = strtok(command, " ");
    int i = 0;
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // NULL-terminate the argument list
}
int load_myshrc_functions(char *input, char *commands[2])
{
    FILE *file = fopen(".myshrc", "r");
    if (file == NULL)
    {
        perror("Could not open .myshrc");
        exit(1);
    }

    char line[MAX_LINE];
    int found_function = 0;

    while (fgets(line, sizeof(line), file))
    {
        // Look for the function definition
        if (strstr(line, input) != NULL)
        {
            found_function = 1; // We found the function

            // Skip to the next two lines to get the commands
            for (int i = 0; i < 2 && fgets(line, sizeof(line), file); i++)
            {
                // Remove any leading/trailing whitespace
                line[strcspn(line, "\n")] = 0;               // Remove newline
                char *command_start = strchr(line, ' ') + 1; // Get the command after '{'
                commands[i] = strdup(command_start);         // Store the command
            }
            break;
        }
    }

    fclose(file);

    if (!found_function)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int execute_function(char *input, char *home, char *last)
{
    char *function_name;
    char *arg;
    function_name = strtok(input, " ");
    // Use strtok to get the second token (arg)
    arg = strtok(NULL, " ");
    char *commands[2] = {NULL, NULL};

    // Load the function commands from the .myshrc file
    int hehehe = load_myshrc_functions(function_name, commands);
    if (hehehe == 0)
    {
        return 0;
    }
    if (commands[0])
    {
        char *result;
        // execute_command(commands[0], arg); // Execute the first command
        sprintf(result, "%s %s", commands[0], arg);
        execute_command(result, home, last);
    }

    if (commands[1])
    {
        char *result;

        sprintf(result, "%s %s", commands[0], arg);
        execute_command(result, home, last); // Execute the second command
    }
    return 1;
    // Free dynamically allocated command strings
    free(commands[0]);
    free(commands[1]);
}
