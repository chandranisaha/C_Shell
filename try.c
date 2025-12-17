#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ignore_sigstp(int signum) {
    printf("Ctrl + Z pressed, but it won't suspend the process.\n");
}

int main() {
    // Ignore the SIGTSTP (Ctrl + Z) signal
    signal(SIGTSTP, ignore_sigstp);

    while (1) {
        printf("Running...\n");
        sleep(1);
    }

    return 0;
}
