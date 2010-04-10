#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>

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
			printf("The child made a system call %ld\n", ret);
			if( ret == 5 ) {
				printf("The child made a system call %ld\n", ret);
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
