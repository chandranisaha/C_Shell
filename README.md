# Shell Implementation in C

## Chandrani Saha

This repository contains a custom Unix-style shell implemented in C, designed to replicate core behaviors of standard command-line shells. The project focuses on low-level systems programming concepts such as process creation, signal handling, command parsing, and I/O redirection.

## Overview

This shell provides an interactive command-line interface that supports execution of both built-in and external commands. It was developed to gain hands-on experience with operating system internals and Unix process management.

Key learning areas include:

- Process control (fork, exec, wait)

- Signal handling

- File descriptors and I/O redirection

- Command parsing and job control

## Getting Started
**Prerequisites**

A C compiler (such as GCC)
Installation guide: https://gcc.gnu.org/install/index.html

## Compilation and Execution
**Repository Setup**
git clone https://github.com/chandranisaha/C_Shell.git
cd C_Shell

**Build**
make


Note: Some functionalities may behave differently when invoked strictly through the Makefile.
For complete functionality, it is recommended to run the compilation command specified inside the Makefile directly in the terminal.

After compilation, run the generated executable to start the shell.

## Shell Functionality

Once launched, the shell displays an interactive prompt and waits for user input. It supports execution of standard Unix commands as well as custom built-in commands implemented as part of the project.

The shell supports:

- Foreground and background process execution

- Command parsing

- Signal handling for user interrupts

- Input/output redirection and piping

## Built-in Commands and Features
**hop**

Changes the current working directory. Supports:

- .

- ..

- ~ (home directory)

- - (previous directory)

**reveal**

Lists directory contents with support for:

- -a (show hidden files)

- -l (detailed listing)

Output is color-coded for better readability.

**log**

Maintains a history of recently executed commands. Supports:

-Viewing command history

- Executing commands from history

- Clearing stored history

**proclore**

Displays information about a given process, including:

- Process ID

- Status

- Memory usage

- Executable path

**seek**

Searches for files or directories with optional filters:

- -d (directories only)

- -f (files only)

- -e (exact match)

**activities**

Lists currently running background processes started by the shell.

**iMan**

Fetches and displays manual pages from an online source.

## Job Control and Signals

The shell supports basic job control and signal handling:

- Ctrl + C: Interrupts the currently running foreground process

- Ctrl + Z: Stops the foreground process and moves it to the background

- Ctrl + D: Exits the shell after cleanup

## Commands:

fg <pid>: Bring a background process to the foreground

bg <pid>: Resume a stopped process in the background

## Design Philosophy

- Minimal and modular C code

- Explicit use of system calls

- Focus on correctness and clarity

- Designed for learning and extensibility rather than production use

## Limitations

- No advanced shell scripting support

- Limited job control compared to full-featured shells like Bash

- No tab completion or configurable prompts

- Basic error handling

## Future Improvements

Possible extensions include:

- Persistent history across sessions

- Tab completion

- Environment variable support

- Improved error handling and diagnostics

---
