
#ifndef REVEAL_H
#define REVEAL_H

#include <sys/stat.h>

void reveal(char *adress,char *home,char* last);
void listDirectory(char *path, int flagA, int flagL);
void printDetails(struct stat *fileStat, char *name);
void printColor(char *name, mode_t mode);



#endif // REVEAL_H
