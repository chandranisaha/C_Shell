#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include "proclore.h"

void proclore_shell(pid_t pid)
{
    // printf("here\n");
    char proc_path[256];
    char status_file[256];
    char exe_path[270];
    char line[256];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d", pid);
    // printf("%s\n",proc_path);

    if (snprintf(status_file, sizeof(status_file), "%s/stat", proc_path) >= sizeof(status_file))
    {
        // Handle the error: The result was truncated
        fprintf(stderr, "Error: status_file buffer too small!\n");
        return;
    }

    if (snprintf(exe_path, sizeof(exe_path), "%s/exe", proc_path) >= sizeof(exe_path))
    {
        // Handle the error: The result was truncated
        fprintf(stderr, "Error: exe_path buffer too small!\n");
        return;
    }
    FILE *f = fopen(status_file, "r");
    if (!f)
    {
        perror("Error opening stat file");
        exit(EXIT_FAILURE);
    }
    int ppid, pgrp, session, tty_nr, tpgid;
    unsigned long vsize;
    char state;
    char comm[256];

    fscanf(f, "%d %s %c %d %d %d %d %d %lu", &pid, comm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid, &vsize);
    fclose(f);

    // Executable path
    char exec_path[270];
    snprintf(exe_path, sizeof(exe_path), "%s/exe", proc_path);
    if (snprintf(exe_path, sizeof(exe_path), "%s/exe", proc_path) >= sizeof(exe_path))
    {
        // Handle the error: The result was truncated
        fprintf(stderr, "Error: exe_path buffer too small!\n");
        return;
    }
    ssize_t len = readlink(exe_path, exec_path, sizeof(exec_path) - 1);
    if (len != -1)
    {
        exec_path[len] = '\0'; // Null-terminate the string
    }
    else
    {
        // Handle error from readlink
        perror("Error reading symbolic link");
        return;
    }
    // Determine process status with foreground/background indicator
    char process_status[3] = {state, '\0', '\0'};
    if (tpgid == pid)
    {
        strcat(process_status, "+");
    }
    printf("PID: %d\n", pid);
    printf("Process Status: %s\n", process_status);
    printf("Process Group: %d\n", pgrp);
    printf("Virtual Memory: %lu\n", vsize);
    printf("Executable Path: %s\n", exec_path);
}

void proc(char *command)
{
    pid_t pid;
    pid_t pid1;

    command += 8;
    while (*command == ' ' || *command == '\t')
    {
        command++;
    }
    if (*command == '\n' || *command == '\0' || command == NULL)
    {
        pid = getpid(); // Default to current process
    }
    else
    {
        // printf("test\n");
        // printf("____%s\n",command);
        pid1 = atoi(command);
        // printf("%d",pid);
        pid = (pid_t) pid1;
    }
    printf("%d\n",pid);
    proclore_shell(pid);
}
