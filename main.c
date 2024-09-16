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
    pid_t children[100];                  // Array of child PIDs
    char root_path[PATH_MAX];         // Root path
    char current_path[PATH_MAX];      // Current working directory
};

struct pid_info temp;

void print_info(struct pid_info *info)
{
        printf("Process Information:\n");
        printf("    Process PID      : %d\n", info->pid);
        printf("    Parent PID       : %d\n", info->parent_pid);
        printf("    Process State    : %u\n", info->state);
        printf("    Stack Pointer    : %lx\n", info->stack_pointer);
	for (int i = 0; i < 16; i++) {
		if (info->children[i] > 0)
        		printf("    Child %d, pid     : %d\n", i, info->children[i]);
	}
        printf("    Current Path     : %s\n", info->current_path);
        printf("    Root Path        : %s\n", info->root_path);
	for (int i = 0; i < 16; i++) {
		if (info->children[i] > 0) {
			int ret = syscall(__NR_SYSCALL, &temp, info->children[i]);
			if (ret == 0)
				print_info(&temp);
		}
	}

}

int main(int argc, char **argv) {
    struct pid_info rets;

    if (argc != 2)
        return 0;

    int pid = atoi(argv[1]);

    int ret = syscall(__NR_SYSCALL, &rets, pid);

    if (ret) {
        printf("Error: %d\n", ret);
    } else {
	    print_info(&rets);
    }

    return 0;
}

