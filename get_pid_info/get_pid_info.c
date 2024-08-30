#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/uaccess.h>  // for copy_to_user and copy_from_user
#include <linux/syscalls.h> // For pid_info definition and syscall declaration
#include <linux/time.h>     // for jiffies
			    //

struct pid_info {
    pid_t pid;                       // Process ID
    long state;                      // Process state
    unsigned long stack_pointer;     // Stack pointer
    unsigned long age;               // Age in jiffies
    pid_t parent_pid;                // Parent PID
    pid_t children[16];              // Array of child PIDs (size could be adjusted)
    char root_path[PATH_MAX];        // Root path
    char current_path[PATH_MAX];     // Current working directory
};

// Define the system call with two arguments
SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, user_info, int, pid) {
	(void)user_info;
	(void)pid;
	return 0;
}

