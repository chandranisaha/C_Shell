

# Shell Implementation in C!

This project is a basic implementation of a shell in the C programming language. The shell provides a command-line interface for users to interact with the operating system.

## Getting Started

### Prerequisites

- Ensure you have [C compiler](https://gcc.gnu.org/install/index.html) installed.

### Building

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/c-shell.git
   cd c-shell
   ```

2. Starting the shell:  
   - Run the command present in the makefile directly in the terminal - some functionalities hamper while running makefile
   - For general purpose run the makefle
```bash 
    make 
```

## Using The Shell


### 1. Prompt Display
The shell displays a custom prompt indicating the current user, system name, and working directory. The home directory is represented as "~," and paths are displayed relative to it.  
Maximum input length has been taken as 100000.

### 2. Command Separation
Commands can be entered using either ; or & as separators to execute multiple commands in sequence or concurrently.

### 3. hop Command
The hop command changes the current working directory. It supports flags such as ".", "..", "~", and "-" for navigation. Multiple hop commands can be executed sequentially when provided with multiple arguments.

```bash
hop test
hop assignment
hop ~
hop -
hop .. tutorial
hop ~/project
```


### 4. reveal Command
The reveal command lists files and directories in a specified directory. It supports flags -a and -l for displaying hidden files and additional information. Differentiates file types with color coding (green for executables, white for files, and blue for directories).
Color is given only to the file/directory name
```bash
 
reveal test
reveal -a -l directory
```

### 5. log Command
The log command stores and retrieves the 15 most recent unique command statements. It supports the execute option to run a command from the past by index and provides a purge option to clear all stored past events.
It also stores erroneous commands.
The history.txt file for storing the information is created on my home directory.
```bash
 
log
log execute 3
log purge
```

### 6. System Commands
The shell executes system commands both in the foreground and background. It captures and displays the process ID (PID) of background processes.

```bash
 
ls
sleep 5 &
echo "Hello"
```
### 7. proclore Command
The proclore command displays information about a specified process, including PID, process status, process group, virtual memory, and executable path.
The units for virutal space are in bytes.
```bash
 
proclore
proclore 123
```

### 8. seek Command
The seek command searches for files or directories in the specified target directory. It supports flags -d, -f, and -e for filtering results.

```bash
 
seek filename
seek -d directory
seek -f filename directory
seek -e -f filename directory
```
### 9. .myshrc Command
This functionality enables the user to alias commands that are mentioned in the `.myshrc` file. 
For aliased commands the log file stores the "alias name".


### 10. I/O Redirection
The shell supports I/O redirection using >, >>, and <. It creates or appends to output files and handles input files appropriately.
ASSUMTION: input output redirection is used only at end of commands, and "&" is taken to be for the particular part of pipe in which it is mentioned.
```bash
 
echo "Hello" > output.txt
cat < input.txt
command >> log.txt
```

### 11. Pipes
The shell implements pipes (|) for passing information between commands. It supports any number of pipes for both user-defined and system commands.

```bash
command1 | command2
command1 | command2 | command3
```

### 12. Redirection with Pipes
Enables I/O redirection along with pipes for user-defined and system commands.

```bash

 
command1 < input.txt | command2 > output.txt
```
### 13. activities Command
The activities command displays a list of currently running processes spawned by the shell. It shows PID, command name, and state (running or stopped).

```bash

 
activities
```
### 14. Signals
The ping command sends signals to processes based on PID and signal number. It displays success messages or an error if the process is not found.

```bash 
 
ping 123 9
ping 456 15
```


Following 3 commands are direct keyboard input where Ctrl is Control key on keyboard (or it’s equivalent).

#### Ctrl - C

- Interrupt any currently running foreground process by sending it the SIGINT signal. It has no effect if no foreground process is currently running.

#### Ctrl - D

- Log out of your shell (after killing all processes) while having no effect on the actual terminal.

#### Ctrl - Z

- Push the (if any) running foreground process to the background and change it’s state from “Running” to “Stopped”. It has no effect on the shell if no foreground process is running.



### 15. fg and bg Commands
The fg command brings a background process to the foreground, and the bg command changes the state of a stopped background process to running.

```bash

 
fg 123
bg 456
```
### 16. Neonate Command
The Neonate command prints the PID of the most recently created process at intervals until the 'x' key is pressed.

```bash
 
neonate -n 4
```

### 17. iMan Command
The iMan command fetches and displays man pages from http://man.he.net/. It accepts a command name as an argument and retrieves the corresponding man page.

```bash
 
iMan ls
iMan grep
```


