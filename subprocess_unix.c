#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <asm/ptrace-abi.h>
#include <sys/wait.h>

void build(char *command)
{
	char l;
	int i, status;
	long orig_eax, name,c;
	pid_t child;

	child = fork();
	if( child == 0 ) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl(command, command, NULL);
	} else {
		while(1) {
			wait(&status);
			if(WIFEXITED(status)) break;
			orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
			name = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
			if( orig_eax == 5 ) {
				int done;
				printf("The child opened ");
				done = 0;
				while( !done ) {
					c = ptrace(PTRACE_PEEKDATA, child, name, NULL);
					for( i = 0; i < 4; i ++ ) {
						l = c & 0xFF;
						c >>= 8;
						name ++;
						if( l == 0 ) {
							done = 1;
							break;
						}
						printf("%c", l);
					}
				}
				printf("\n");
			}
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
	}
}

int main(int argc, char *argv[])
{
	if( argc >= 2 ) {
		build(argv[1]);
	} else {
		build("/bin/ls");
	}

	return 0;
}
