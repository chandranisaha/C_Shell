#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/select.h>
#include <termios.h>

// Global flag to signal when to stop printing
int stop = 0;
struct termios oldt, newt;

// Signal handler for 'x' key press
void stop_on_x()
{
    stop = 1;
}

// Function to find the most recently created PID
pid_t get_newest_pid()
{
    signal(SIGINT, stop_on_x);
    DIR *proc_dir;
    struct dirent *entry;
    pid_t newest_pid = 0;
    pid_t current_pid;

    // Open /proc directory
    proc_dir = opendir("/proc");
    if (proc_dir == NULL)
    {
        perror("opendir(/proc) failed");
        exit(1);
    }

    // Iterate over the entries in /proc
    while ((entry = readdir(proc_dir)) != NULL)
    {
        // Check if the directory name is a number (i.e., a PID)
        if (isdigit(entry->d_name[0]))
        {
            current_pid = atoi(entry->d_name);
            if (current_pid > newest_pid)
            {
                newest_pid = current_pid;
            }
        }
    }

    closedir(proc_dir);
    return newest_pid;
}

// Function to wait for 'x' key press asynchronously
int check_for_x_key()
{
    struct timeval tv;
    fd_set fds;
    int result;

    // Set up a timeout of 0.05 seconds to check for keypress
    tv.tv_sec = 0;
    tv.tv_usec = 50000; // 0.05 seconds

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); // Monitor standard input for keypress

    result = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    if (result > 0)
    {
        char c = getchar(); // Read the input character
        if (c == 'x' || c == 'X')
        {

            return 1; // Return 1 if 'x' or 'X' is pressed
        }
    }
    return 0; // No 'x' keypress detected
}

// Function to restore terminal settings
void reset_terminal_mode()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

// Function to set terminal to unbuffered mode
void set_conio_terminal_mode()
{
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // newt.c_iflag &= ~(IXON | IXOFF | IXANY);  // Turn off software flow control

    // 2. Output flags (c_oflag): Modify output processing flags
    //    We don't need any major changes to `c_oflag` for character-by-character input/output.
    // newt.c_oflag &= 1; // Disable post-processing of output (e.g., converting \n to \r\n)

    // 3. Control flags (c_cflag): Ensure proper character size and disable other unnecessary features
    //    These flags primarily deal with hardware control.
    newt.c_cflag |= (CLOCAL | CREAD);  // Ensure the port is local and read is enabled
    newt.c_cflag &= ~CSIZE;            // Mask the character size bits
    newt.c_cflag |= CS8;               // Use 8-bit characters

    // 4. Local flags (c_lflag): This is where we disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
}

void neonate(int time_arg)
{
    if (time_arg < 0)
    {
        printf("Negative number not allowed\n");
        return;
    }
    signal(SIGINT, stop_on_x);

    // Set terminal to unbuffered mode
    set_conio_terminal_mode();

    while (!stop)
    {
        // Fetch the most recently created PID
        pid_t newest_pid = get_newest_pid();

        // Print the PID
        printf("%d\n", newest_pid);

        if (time_arg == 0)
        {
            // Continuous output case
            if (check_for_x_key())
            {
                stop = 1;
                break;
            }
            usleep(100000); // Sleep for 0.1 seconds to avoid overwhelming the CPU
        }
        else
        {
            // Sleep for the specified time, checking for 'x' key press periodically
            int elapsed_time = 0;
            while (elapsed_time < time_arg)
            {
                if (check_for_x_key())
                {
                    stop = 1;
                    break;
                }
                sleep(1); // Sleep for 1 second at a time to allow checking for key press
                elapsed_time++;
            }
        }
    }
    stop = 0;

    // Restore terminal settings
    reset_terminal_mode();
}
