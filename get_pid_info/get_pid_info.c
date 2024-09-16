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
#include <linux/jiffies.h>
			    //

struct pid_info {
    pid_t pid;                       // Process ID
    unsigned int state;                      // Process state
    unsigned long stack_pointer;     // Stack pointer
    unsigned long age;               // Age in jiffies
    pid_t parent_pid;                // Parent PID
    pid_t children[100];              // Array of child PIDs (size could be adjusted)
    char root_path[PATH_MAX];        // Root path
    char current_path[PATH_MAX];     // Current working directory
};

// Define the system call with two arguments
SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, user_info, int, pid) 
{
	struct task_struct	*task;
	struct task_struct	*child;
	struct pid_info		*ret_info;
	int children_count = 0;

	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if (!task)
		return -ESRCH;
	ret_info = kzalloc(sizeof(struct pid_info), GFP_KERNEL);
	rcu_read_lock();
	ret_info->pid = task->pid;
	ret_info->state = task->__state;
	ret_info->stack_pointer = (unsigned long) task->stack;
	ret_info->parent_pid = task->parent->pid;
	ret_info->age = jiffies - task->start_time;
	list_for_each_entry(child, &task->children, sibling)
	{
		ret_info->children[children_count++] = child->pid;
		if (children_count > 15) break;
	}
	rcu_read_unlock();

	spin_lock(&task->fs->lock);
	strncpy(ret_info->current_path, dentry_path_raw(task->fs->pwd.dentry, ret_info->current_path, PATH_MAX), PATH_MAX);
	strncpy(ret_info->root_path, dentry_path_raw(task->fs->root.dentry, ret_info->root_path, PATH_MAX), PATH_MAX);
	spin_unlock(&task->fs->lock);
	// missing full patch and current path

	int ret = copy_to_user(user_info, ret_info, sizeof(struct pid_info));
	if (ret)
		pr_info("We didn't copy the whole thing\n");
	kfree(ret_info);
	return 0;
}

