#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "seek.h"
#include "hop.h"
int bleh = 0;
// int e_flag=0;
char jump[2096];
char target_actual[1024];



int cmp(const char *a, const char *b)
{
    int len = strlen(b);
    if(strlen(a)<len)
    return 1;

    for (int i = 0; i < len; i++)
    {
        if (a[i] != b[i])
            return 1;
    }
    return 0;
}

// char *search_directory(const char *base_path, const char *target, int f, int d, int e, int *no)
char *search_directory(const char *base_path, const char *target, int f, int d, int e, int *no, char *home, char *last)
{
    // YES;
    char *ans;
    struct dirent *dp;
    struct stat statbuf;
    // printf("%s and %s\n", base_path, target);
    if (access(base_path, R_OK | X_OK) != 0)
    {
        perror("Permission denied or directory not accessible");
        return NULL;
    }

    DIR *dir = opendir(base_path);
    if (dir == NULL)
    {
        perror("Could not open directory");
        return NULL;
    }
    // printf("%s and %s\n", base_path,target);

    if (!dir)
    {
        perror("Unable to read directory");

        return NULL;
    }

    char full_path[1024];
    int found_file = 0, found_dir = 0;
    char file_match[1024], dir_match[1024];

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, dp->d_name);
        if (stat(full_path, &statbuf) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            if (!f && cmp(dp->d_name, target) == 0)
            {
                printf("\033[1;34m%s/%s\033[0m\n", base_path, dp->d_name);
                bleh = 1;
                found_dir += 1;
                strcpy(dir_match, full_path);
            }
            // else
            // {
            //     printf("No Match Found\n");
            // }
            ans = search_directory(full_path, target, f, d, e, no, home, last);
        }
        else if (S_ISREG(statbuf.st_mode))
        {
            if (!d && cmp(dp->d_name, target) == 0)
            {
                printf("\033[1;32m%s/%s\033[0m\n", base_path, dp->d_name);
                bleh = 1;
                found_file += 1;
                strcpy(file_match, full_path);
            }
            // else
            // {
            //     printf("No Match Found\n");

            // }
        }
    }

    closedir(dir);

    // printf("%d %d",found_dir,found_file);
    // YES;

    if (e)
    {
        if (found_file == 1 && !found_dir)
        {
            if (access(file_match, R_OK) == -1)
            {
                printf("Missing permissions for task!\n");
                bleh = 1;
            }
            else
            {
                char line[1024];
                FILE *file = fopen(file_match, "r");

                while (fgets(line, sizeof(line), file))
                {
                    // Print each line
                    printf("%s", line);
                    bleh = 1;
                }
                printf("\n");
                fclose(file);
            }
        }
        else if (found_dir == 1 && !found_file)
        {
            // char * buff;
            char buff[1040]; // Allocate enough space
            sprintf(buff, "hop %s", dir_match);
            // printf("%s\n", buff);
            if (access(dir_match, X_OK) != 0)
            {
                perror("Permission denied or directory not accessible");
                return NULL;
            }
            sprintf(jump, "%s", dir_match);

            // e_flag=1;
            hop(buff, home, last);
            bleh=1;
            // printf("%s",dir_match);
            // printf("hello\n");
            ans = malloc(sizeof(char) * 1024);
            strcpy(ans, dir_match);

            // printf("%d\n",a);
            // YES;
            (*no)++;

            // *filematch = file_match;
            // printf("%s\n", dir_match);
            bleh = 1;
        }
    }
    // if (bleh == 0)
    // {
    //     printf("No Match Found\n");
    // }
    return ans;
}

void seek_command(const char *adress, char *home, char *last)

{
    // printf("HELLO\n");
    int d = 0;
    int f = 0;
    int e = 0;
    adress = adress + 4;
    while (*adress == ' ' || *adress == '\t')
    {
        adress++;
    }
    int flag = 1;
    while ((*adress == '-') || (flag == 0 && *adress != ' ' || (*adress == ' ')))
    {
        if (*adress == ' ' || *adress == '-')
        {
            // printf("helo\n");
            if (*adress == '-' && (*(adress + 1) != 'd' && *(adress + 1) != 'f' && *(adress + 1) != 'e'))
            {
                // printf("helobhb\n");
                break;
            }
            adress++;
            // printf("%c\n\n",*adress);
        }
        flag = 0;
        
        if (*adress == 'd')
        {
            if (*(adress - 1) == ' ')
            {
                // printf("hee  d \n");

                break;
            }
            d = 1;
            // printf("done D\n");

            adress++;
        }
        else if (*adress == 'f')
        {
            if (*(adress - 1) == ' ')
            {
                // printf("hee f  \n");

                break;
            }
            f = 1;
            // printf("done F\n");
            adress++;
        }
        else if (*adress == 'e')
        {
            if (*(adress - 1) == ' ')
            {
                break;
            }
            e = 1;
            adress++;
        }
        else if (*adress == ' ' || *adress == '-')
        {
            // printf("hee  \n");
            adress++;
        }
        else
        {
            // printf("hefde  \n");

            break;
        }

        // Skip any spaces or tabs after the flag
    }
    // YES;
    while (*adress == ' ' || *adress == '\t')
    {
        adress++;
    }

    const char *target = adress;
    const char *buffer = target;
    const char *buffer2 = target;
    char *search_path;
    const char *ss;
    while (*buffer != ' ' && *buffer != '\t' && *buffer != '\0')
    {
        buffer++;
    }
    // YES;

    if (*buffer == '\0')
    {
        *search_path = '.';
        search_path++;
        *search_path = '\0';
        search_path--;
        ss = search_path;
        strcpy(target_actual, target);
    }
    else
    {
        ss = buffer;
        // buffer2++;/
        int countermy = 0;

        while (buffer2[countermy] != ' ' && buffer2[countermy] != '\0')
        {
            target_actual[countermy] = buffer2[countermy];
            countermy++;
        }
        target_actual[countermy] = '\0';
        ss++;
        // const char* target2 = target;
    }
    // YES;

    // printf("%d %d %d\n", d, e, f);

    int dirno = 0;
    if (f == 1 && d == 1)
    {
        printf("Invalid flag combination\n");
        return;
    }

    search_directory(ss, target_actual, f, d, e, &dirno, home, last);

    if (bleh == 0)
    {
        printf("No Match Found\n");
    }
    // int y = chdir(maimarungajump);
    // if (chdir(maimarungajump) == 0)
    // {
    //     // chdir(adress);
        
    //     // char cwd[1024];
    //     // if (getcwd(cwd, sizeof(cwd)) != NULL)
    //     // {
    //     //     printf("Changed directory to: %s\n", cwd);
    //     // }
    //     // else
    //     // {
    //     //     perror("getcwd() error");
    //     // }
    // }
    // else
    // {
    //     // printf("hello\n");
    //     perror("chdir() failed");
    // }

    return;
}