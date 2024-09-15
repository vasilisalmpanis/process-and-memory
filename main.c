#include <sys/syscall.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define PATH_MAX 4096
#define __NR_SYSCALL 335

struct pid_info {
    pid_t pid;                        // Process ID
    unsigned int state;               // Process state
    const unsigned long stack_pointer; // Stack pointer
    unsigned long age;                // Age in jiffies
    pid_t parent_pid;                 // Parent PID
    pid_t *children;                  // Array of child PIDs
    char root_path[PATH_MAX];         // Root path
    char current_path[PATH_MAX];      // Current working directory
};

int main(int argc, char **argv) {
    struct pid_info rets;

    // Check if the correct number of arguments is passed
    if (argc != 2)
        return 0;

    // Convert the argument to an integer representing the PID
    int pid = atoi(argv[1]);

    // Make the system call
    int ret = syscall(__NR_SYSCALL, &rets, pid);

    // Check if the system call succeeded
    if (ret) {
        printf("Error: %d\n", ret);
    } else {
        // Print the process details in a pretty format
        printf("Process Information:\n");
        printf("    Process PID      : %d\n", rets.pid);
        printf("    Parent PID       : %d\n", rets.parent_pid);
        printf("    Process State    : %u\n", rets.state);
        printf("    Stack Pointer    : %lx\n", rets.stack_pointer);
    }

    return 0;
}

