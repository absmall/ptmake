#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/ptrace.h>
#include <asm/ptrace-abi.h>
#include <sys/wait.h>

using namespace std;

void run(string command, void (*callback)(string filename))
{
	char l;
	int i, status;
	long orig_eax, name,c;
	pid_t child;

	child = fork();
	if( child == 0 ) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl(command.c_str(), command.c_str(), NULL);
	} else {
		while(1) {
			wait(&status);
			if(WIFEXITED(status)) break;
			orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
			name = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
			if( orig_eax == 5 ) {
				int done;
				string s;
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
						s += l;
					}
				}
				callback(s);
			}
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
	}
}
