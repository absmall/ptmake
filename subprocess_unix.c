#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <asm/ptrace-abi.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	long orig_eax;
	pid_t child;

	child = fork();
	if( child == 0 ) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl("/bin/ls", "ls", NULL);
	} else {
		wait(NULL);
		orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
		printf("The child made a system call %ld\n", orig_eax);
		ptrace(PTRACE_CONT, child, NULL, NULL);
	}
	return 0;
}
