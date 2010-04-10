#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#include "subprocess.h"

void trace(const char *command)
{
	int status;
	long ret;
	pid_t child;

	child = fork();
	if( child == 0 ) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		printf("Tracing %s\n", command);
		
		execl("/bin/sh", "sh", "-c", command, NULL);
	} else {
		while(1) {
			wait(&status);
			if(WIFEXITED(status)) break;
#if defined(__i386)
			ret = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
#elif defined(__x86_64)
			ret = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
#endif
			switch( ret ) {
				case __NR_stat:
					printf("Stat call\n");
					break;
				case __NR_fstat:
					printf("fstat call\n");
					break;
				case __NR_open:
					printf("Open call\n");
					break;
				case __NR_access:
					printf("Access call\n");
					break;
				case __NR_fcntl:
					printf("Fcntl call\n");
					break;
				case __NR_arch_prctl:
					printf("Arch prctl call\n");
					break;
				case __NR_execve:
					printf("Execve call\n");
					break;
				case __NR_dup2:
					printf("Dup2 call\n");
					break;
				case __NR_exit_group:
					printf("Exit group call\n");
					break;
				// Don't care about these ones
				case __NR_mmap:
				case __NR_mprotect:
				case __NR_getppid:
				case __NR_brk:
				case __NR_geteuid:
				case __NR_getpid:
				case __NR_munmap:
				case __NR_read:
				case __NR_write:
				case __NR_rt_sigaction:
				case __NR_close:
					break;
				default:
					printf("The child made a system call %ld\n", ret);
					break;
			}
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
	}
}

#if 0
int main(int argc, char *argv[])
{
	if( argc >= 2 ) {
		trace(argv[1]);
	} else {
		trace("/bin/ls");
	}
	return 0;
}
#endif
