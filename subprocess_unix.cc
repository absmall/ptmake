#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/ptrace.h>
#include <asm/ptrace-abi.h>
#include <sys/wait.h>
#include <syscall.h>
#include "subprocess.h"

using namespace std;

void trace(string command, void (*callback)(string filename))
{
	char l;
	int i, status;
	long syscall_id, name,c;
	pid_t child;

	child = fork();
	if( child == 0 ) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl(command.c_str(), command.c_str(), NULL);
	} else {
		while(1) {
			wait(&status);
			if(WIFEXITED(status)) break;
#if defined(__i386)
			syscall_id = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
#elif defined(__x86_64)
			ret = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
#endif
			name = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
			switch( syscall_id ) {
				case __NR_open:
				{
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
				case __NR_stat:
					cout << "Stat call" << endl;
					break;
				case __NR_fstat:
					cout << "fstat call" << endl;
					break;
				case __NR_access:
					cout << "Access call" << endl;
					break;
				case __NR_fcntl:
					cout << "Fcntl call" << endl;
					break;
#if defined(__x86_64)
				case __NR_arch_prctl:
					cout << "Arch prctl call" << endl;
					break;
#endif
				case __NR_execve:
					cout << "Execve call" << endl;
					break;
				case __NR_dup2:
					cout << "Dup2 call" << endl;
					break;
				case __NR_exit_group:
					cout << "Exit group call" << endl;
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
					cout << "The child made a system call " << syscall_id << endl;
					break;
			}
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
	}
}
