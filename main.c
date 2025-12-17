#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include "input_handler.h"
#include "log.h"
#include <sys/utsname.h>
#include "activities.h"
#include <pwd.h>
#include "seek.h"
#include <sys/types.h>
#include "keyboard.h"
#include "ping.h"
#include <setjmp.h>
#include "aliasing.h"
// sigjmp_buf env;

int time1;
int sigint_flag = 0;

int logon;

int cmp3(const char *a, const char *b)
{
    // printf("%s and %s\n",a,b);
    int len = (strlen(a) > strlen(b)) ? strlen(b) : strlen(a);

    if (strlen(a) != strlen(b))
        return 1;

    for (int i = 0; i < len; i++)
    {
        if (a[i] != b[i])
            return 1;
    }
    return 0;
}

int main()
{

    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
    // signal(,logout_shell)

    //  check for error msgs of hop- DONE
    // reveal relative paths, without l or a tag and lexicographic order -DONE
    // debug history / log  and add pruge and execute thingy - DONE
    // log not working properly (more than 15 and repetiotion) -DONE
    //  proclore with given id - DONE
    // background process pid and time - DONE
    //  execvp gives error in bg process - DONE
    //  history.txt for execvp commands - DONE

    //  change of directory in seek - child cant change directory of parent
    //  time if it is not the lst command e.g sleep5; echo 5 or sleep 5; sleep 3
    //  seek -e dummy1 , seek -e t  , seek -e -d, seek dummy1 ./testing - DONE
    //  seek -e f
    //  vim& - DONE
    //  zombie process
    //  2 background processes
    //  confusion bg process should give back control to terminal meaning
    // ________________________________________
    //  PART B
    //  input not coming back to terminal after <- DONE
    //  wc < hi.txt > a.txt ___ implement after clarity on how many <  > allowed - DONE
    // bg process not giving back signal and foreground not giving time -DONE
    // echo 4 & wc < hi.txt done
    //  activities not working done
    //  iman soemtimes prints iman echo , sometimes terminates in between ( handle by doing multiple runs)
    //  handle multiple arguments in iman done
    //  ctrl c ctrl z multiple time happening - DONE
    //  check activities ss done

    int count = 0;
    int max_length = 100000;
    char home[PATH_MAX];
    char lastpath[PATH_MAX];
    char current[PATH_MAX];
    if (getcwd(home, sizeof(home)) != NULL)
    {
        getcwd(current, sizeof(current));
    }
    else
    {
        // printf("here");
        perror("getcwd() error");
    }
    // if( e_flag ==1)
    // {
    //     // dbchjab
    // }
    time1 = 0;
    logon = 0;
    lastpath[0] = '\0';
    char *input = malloc(sizeof(char) * (max_length));
    char *input_2 = malloc(sizeof(char) * (max_length));
    memset(input, '`', max_length - 1); // Fill the first max_length-1 positions with '`'
    input[max_length - 1] = '\0';

    // Handle Ctrl+C
    running_forground = 0;
    load_myshrc();
    while (1)
    {
        // sigint_flag = !sigint_flag;
        // printf("Running... (Ctrl+C flag is %s)\n", sigint_flag ? "ON" : "OFF");

        // if (sigsetjmp(env, 1) == 0)
        // {
        //     printf("ok\n");

        // a++;
        // printf("&\n");
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            if (cmp3(current, cwd) == 1)
            {
                // printf("OK\n");
                strcpy(lastpath, current);
                strcpy(current, cwd);
            }
            int countH = 0;
            int countHOME = 0;
            int countC = 0;
            struct passwd *pw;
            uid_t uid = geteuid(); // get the pid of process
            pw = getpwuid(uid);
            char *username = pw ? pw->pw_name : "unknown";
            struct utsname unameData;
            uname(&unameData);
            char *system_name = unameData.nodename; // system name k liye
                                                    // printf("<%s@%s:%s>\n", username, system_name, cwd);
            while (home[countHOME] != '\0' && home[countHOME] != '\n')
            {
                countHOME++;
            }
            while (home[countH] == cwd[countC])
            {
                countH++;
                countC++;
                if (home[countH] == '\0' || home[countH] == '\t' || home[countH] == '\n')
                {
                    break;
                }
            }
            if (countC < countHOME)
            {
                if (time1 < 3)
                    printf("< %s@%s:~%s>", username, system_name, cwd);
                else
                {
                    if (input[0] != '`')
                    {
                        char *proNAME = malloc(sizeof(char) * (max_length));
                        int i;
                        for (i = 0; input[i] != ' ' && input[i] != '\0'; i++)
                        {
                            proNAME[i] = input[i];
                        }
                        proNAME[i] = '\0';
                        printf("check\n");
                        printf("< %s@%s:~%s %s : %d s >", username, system_name, cwd, proNAME, time1);
                        time1 = 0;
                    }
                    else
                    {
                        printf("< %s@%s:~%s>", username, system_name, cwd);
                    }
                }
            }
            // char adr[PATH_MAX];
            else
            {
                char *adr = malloc(sizeof(char) * (PATH_MAX));
                int adrIndex = 0; // Index for the adr array

                while (cwd[countC] != '\0')
                {
                    adr[adrIndex++] = cwd[countC++];
                }

                adr[adrIndex] = '\0';
                // printf("\n%s\n",adr);
                if (time1 < 3)
                    printf("< %s@%s:~%s>", username, system_name, adr);
                else
                {
                    // printf("< %s@%s:~%s sleep : %d >", username, system_name, adr, time1);
                    // time1 = 0;

                    if (input[0] != '`')
                    {
                        char *proNAME = malloc(sizeof(char) * (max_length));
                        int i;
                        for (i = 0; input[i] != ' ' && input[i] != '\0'; i++)
                        {
                            proNAME[i] = input[i];
                        }
                        proNAME[i] = '\0';
                        // printf("check\n");
                        printf("< %s@%s:~%s %s : %d s >", username, system_name, adr, proNAME, time1);
                        time1 = 0;
                    }
                    else
                    {
                        printf("< %s@%s:~%s>", username, system_name, adr);
                    }
                    time1 = 0;
                }
                free(adr);
            }
            // printf("poiugy\n");
            if (feof(stdin))
            {
                logout_shell(); // Log out and terminate all processes
            }
            // printf("poiugy\n");

            scanf(" %4095[^\n]%*c", input);
            // input_2 = input;
            // printf("poiugy\n");

            strcpy(input_2, input);
            // printf("poiugy\n");


            pid_t pid = getpid(); // Get the process ID

            // printf("poiugy\n");

            running_forground = 1;
            // printf("poiugy\n");
// 
            handle_input(input, home, lastpath);
            // printf("out\n");
            running_forground = 0;

            if (logon == 0)
            {
                // printf("%s\n",input_2);

                // printf("%s\n",input);
                // printf("here\n");
                store_history(input_2);
            }
            else
            {
                logon = 0;
            }

            // printf("%d\n",logon);
        }
        else
        {
            // printf("here\n");

            perror("getcwd() error");
        }
    }
}