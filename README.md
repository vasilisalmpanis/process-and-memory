
# Process And Memory
The goal of this project is to write the following not-yet-existing syscall sys_get_pid_info <br>
to gather information about a process, store it, and return it to user space.



## Goals
- Find and understand the syscalls table
- Find and understand the syscalls header file.
- Add a custom syscall in the Linux Kernel. In our case, a syscall that takes a PID
as its argument, fill a structure with informations regarding that PID and send it
back to the caller.
- Integrate your code in the Linux code base.
- Handle correctly the differences between the kernel and userland

### Subject -> process-and-memory-subject.pdf
Syscall Prototype
```
long sys_get_pid_info(struct pid_info *ret, int pid);
```

struct pid_info should contain
```
- PID
- State of the process (Between 3 values)
- Pointer to process’ stack. (RO in user space)
- Age of the process (Since execution, not boot-time)
- An array of child processes (PIDS).
- Parent PID
- Root path of the process
- Current PWD of the process (Full path)
```

### Restrictions
Since the amount of child processes one process can have is random,
the size of the array to store child processes pid_t has to somehow 
be communicated to the syscall. I chose to use a fixed size array 
that can be changed at compilation to not depend on userspace for 
the amount of pid_t I can return.


### TODO:
- Recode kill, wait, mmap and fork
