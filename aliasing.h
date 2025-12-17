#ifndef ALIASING_H
#define ALIASING_H

#define MAX_ALIASES 100
#define MAX_LINE 100

#define MAX_FUNCTIONS 100

// Alias structure definition
typedef struct {
    char alias[50];       // Alias name
    char command[200];    // Command to which alias maps
} Alias;

typedef struct {
    char func_name[50];   // Function name
    char func_body[500];  // Body of the function (commands to execute)
} Function;

// Declare global arrays for aliases and functions
extern Alias aliases[MAX_ALIASES];       // Array to store aliases
extern int alias_count;                  // Number of aliases currently loaded

extern Function functions[MAX_FUNCTIONS]; // Array to store functions
extern int function_count;                // Number of functions currently loaded

// Function prototypes
void load_myshrc();
char *get_alias_command(char *command);
void parse_command(char *command, char **args);
int load_myshrc_functions(char *input, char *commands[2]);
int execute_function(char *input, char *home, char *last);

#endif  // ALIASING_H
