#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <asm/ptrace-abi.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	long orig_eax;
	pid_t child;

	child = fork();
	if( child == 0 ) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		if( argc >= 2 ) {
			execl(argv[1], argv[1], NULL);
		} else {
			execl("/bin/ls", "ls", NULL);
		}
	} else {
		while(1) {
			wait(&status);
			if(WIFEXITED(status)) break;
			orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
			if( orig_eax == 5 ) {
				printf("The child made a system call %ld\n", orig_eax);
			}
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
	}
	return 0;
}
