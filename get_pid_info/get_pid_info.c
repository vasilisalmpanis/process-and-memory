#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/uaccess.h>  // for copy_to_user and copy_from_user
#include <linux/syscalls.h> // For pid_info definition and syscall declaration
#include <linux/time.h>     // for jiffies
#include <linux/rcupdate.h>
#include <linux/slab.h>
			    //

struct pid_info {
    pid_t pid;                       // Process ID
    unsigned int state;                      // Process state
    unsigned long stack_pointer;     // Stack pointer
    unsigned long age;               // Age in jiffies
    pid_t parent_pid;                // Parent PID
    pid_t *children;              // Array of child PIDs (size could be adjusted)
    char root_path[PATH_MAX];        // Root path
    char current_path[PATH_MAX];     // Current working directory
};

// Define the system call with two arguments
SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, user_info, int, pid) 
{
	struct task_struct	*task;
	struct pid_info		*ret_info;

	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if (!task)
		return -ESRCH;
	pr_info("We didn't copy the whole thing\n");
	ret_info = kzalloc(sizeof(struct pid_info), GFP_KERNEL);
	ret_info->pid = task->pid;
	ret_info->state = task->__state;
	ret_info->stack_pointer = (unsigned long) task->stack;
	rcu_read_lock();
	ret_info->parent_pid = task->parent->pid;
	rcu_read_unlock();

	// missing full patch and current path

	int ret = copy_to_user(user_info, ret_info, sizeof(struct pid_info));
	if (ret)
		pr_info("We didn't copy the whole thing\n");
	kfree(ret_info);
	return 0;
}

