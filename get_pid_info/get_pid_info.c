#include <linux/kernel.h>
#include <linux/syscall.h>
#include <linux/pid.h>

struct pid_info {
	int	pid;
	int	state;
	void	*stack;
};

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, ret, int , pid)
{
	struct task_struct *result = get_pid_task(pid , PIDTYPE_PID);
	if (!result)
		return -ESRCH;
	(void) ret;
	printk(KERN_INFO"Hello syscall\n");
	return 0;
}
