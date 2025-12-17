#ifndef SEEK
#define SEEK


#define COLOR_GREEN "\033[0;32m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"
char* search_directory(const char *base_path, const char *target, int f, int d, int e,int*no, char *home,  char *last);
void seek_command(const char *adress, char *home,  char *last);

extern char target_actual[1024];



#define YES printf("YES\n")

#endif