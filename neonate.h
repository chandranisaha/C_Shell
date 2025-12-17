#ifndef NEONATE_H
#define NEONATE_H
extern int stop;
extern struct termios oldt, newt;

// Function declarations

// Signal handler for 'x' key press
void stop_on_x();

// Function to find the most recently created PID
pid_t get_newest_pid();

// Function to wait for 'x' key press asynchronously
int check_for_x_key();

// Function to restore terminal settings
void reset_terminal_mode();

// Function to set terminal to unbuffered mode
void set_conio_terminal_mode();

// Main function to execute the neonate command with a time argument
void neonate(int time_arg);

#endif // NEONATE_H
