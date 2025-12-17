#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "reveal.h"
void printColor(char *name, mode_t mode)
{
    if (S_ISDIR(mode))
    {
        printf("\033[1;34m%s\033[0m\n", name); // Blue for directories
    }
    else if (mode & S_IXUSR)
    {
        printf("\033[1;32m%s\033[0m\n", name); // Green for executables
    }
    else
    {
        printf("%s\n", name); // White for files
    }
}

void printDetails(struct stat *fileStat, char *name)
{
    char permissions[11];
    permissions[0] = S_ISDIR(fileStat->st_mode) ? 'd' : '-';
    permissions[1] = (fileStat->st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (fileStat->st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (fileStat->st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (fileStat->st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (fileStat->st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (fileStat->st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (fileStat->st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (fileStat->st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (fileStat->st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    struct passwd *pw = getpwuid(fileStat->st_uid);
    struct group *gr = getgrgid(fileStat->st_gid);
    char timeStr[100];
    strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", localtime(&fileStat->st_mtime));

    printf("%s %ld %s %s %5ld %s ", permissions, fileStat->st_nlink, pw->pw_name, gr->gr_name, fileStat->st_size, timeStr);
    printColor(name, fileStat->st_mode);
}

int filter(const struct dirent *entry)
{
    return 1; // Accept all entries, filtering is done later based on flags
}

void listDirectory(char *path, int flagA, int flagL)
{
    struct dirent **namelist;
    struct stat fileStat;
    int n;

    n = scandir(path, &namelist, filter, alphasort);
    if (n < 0)
    {
        perror("Unable to open directory");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        struct dirent *entry = namelist[i];

        if (!flagA && entry->d_name[0] == '.')
        {
            free(namelist[i]);
            continue; // Skip hidden files unless -a flag is present
        }

        char fullPath[PATH_MAX];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if (stat(fullPath, &fileStat) < 0)
        {
            perror("Unable to get file status");
            free(namelist[i]);
            continue;
        }

        if (flagL)
        {
            printDetails(&fileStat, entry->d_name);
        }
        else
        {
            printColor(entry->d_name, fileStat.st_mode);
        }

        free(namelist[i]);
    }

    free(namelist);
}
void reveal(char *adress, char *home,char* last)
{
    adress = adress + 6;
    while (*adress == ' ' || *adress == '\t')
    {
        adress++;
    }
    char *directory = adress;
    int flagA = 0;
    int flagL = 0;
    // while (*directory == '-')
    {
        // directory++; // Move past the '-'

        // Process flags
        int flag =0;
        while ((*directory == '-' && (*(directory+1)=='l'|| *(directory+1)=='a'))|| (*directory == 'a' && flag==1) || (*directory == 'l'&& flag==1)  )
        {
            directory++;
            flag=1;
            if (*directory == 'a')
            {
                flagA = 1;
                directory++;
            }
            if (*directory == 'l')
            {
                flagL = 1;
                directory++;
            }
             // Move to the next character
        }


        while(*directory == ' ' || *directory == '\t')
        directory++;

        // After processing the flags, ensure the next character is either a space or end of string


        // Skip spaces after flags


        // Ensure that at least one valid flag is present
        char *path = directory;
        // printf("%s\n",path);
        if (*path == '\0')
        {
            path = ".";
        }
        else if (strcmp(path, "~") == 0)
        {
            path = home;
        }
        else if (strcmp(path, "-") == 0)
        {
            path = last;
            // printf("ok");
        }
        // printf("%s\n",path);


        listDirectory(path, flagA, flagL);
    }
   
}
