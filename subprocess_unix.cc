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
#include "globals.h"

using namespace std;

#ifdef DEBUG
struct
{
	int id;
	const char *statement;
} static debugprints[] = {
	{ __NR_stat, "stat" },
	{ __NR_access, "access" },
	{ __NR_open, "open" },
	{ __NR_geteuid32, "geteuid32" },
	{ __NR_set_thread_area, "set_thread_area" },
	{ __NR_stat64, "stat64" },
	{ __NR_fstat, "fstat" },
	{ __NR_fstat64, "fstat64" },
	{ __NR_mmap2, "mmap2" },
	{ __NR_fcntl, "fcntl" },
#if defined(__x86_64)
	{ __NR_arch_prctl, "arch prctl" },
#endif
	{ __NR_execve, "execve" },
	{ __NR_dup2, "dup2" },
	{ __NR_exit_group, "exit group" },
	{ __NR_mmap, "mmap" },
	{ __NR_mprotect, "mprotect" },
	{ __NR_getppid, "getppid" },
	{ __NR_brk, "brk" },
	{ __NR_geteuid, "geteuid" },
	{ __NR_getpid, "getpid" },
	{ __NR_munmap, "munmap" },
	{ __NR_read, "read" },
	{ __NR_write, "write" },
	{ __NR_rt_sigaction, "rt_sigaction" },
	{ __NR_close, "close" },
	{ __NR_clone, "clone" },
	{ __NR_wait4, "wait4" },
	{ __NR_unlink, "unlink" },
};

void debugprint( int pid, int syscall_id, int returnVal )
{
	unsigned int i;

	for( i = 0; i < sizeof(debugprints)/sizeof(debugprints[0]); i ++ ) {
		if( debugprints[ i ].id == syscall_id ) {
			cout << pid << ": " << debugprints[ i ].statement << " call (" << returnVal << ")" << endl;
			return;
		}
	}
	cout << pid << " made a system call " << syscall_id << " returning " << returnVal << endl;
}
#endif

void Subprocess::trace(string command)
{
	char l;
	int i, status;
	long syscall_id, name,c, returnVal;
	pid_t child;
	bool insyscall = false;

	cout << "Executing " << command << endl;

	child = fork();
	if( child == 0 ) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl("/bin/sh", "sh", "-c", command.c_str(), NULL);
	} else {
		while(1) {
			child = wait(&status);
			ptrace(PTRACE_SETOPTIONS, child, NULL, PTRACE_O_TRACEFORK | PTRACE_O_TRACEVFORK | PTRACE_O_TRACECLONE | PTRACE_O_TRACEEXEC);
			if(WIFEXITED(status)) break;
#if defined(__i386)
			syscall_id = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
#elif defined(__x86_64)
			ret = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
#endif
			name = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
			returnVal = ptrace(PTRACE_PEEKUSER, child, 4 * EAX, NULL);
#ifdef DEBUG
			if( debug ) {
				debugprint( child, syscall_id, returnVal );
			}
#endif
			switch( syscall_id ) {
				case __NR_stat:
				case __NR_access:
				case __NR_open:
				case __NR_stat64:
				{
					int done;
					string s;

					insyscall ^= 1;
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
					if( insyscall ) {
						callback_entry(s);
					} else {
						callback_exit(s, returnVal >= 0);
					}
				}
			}
			if( syscall_id == __NR_exit_group ) {
				// Detach here - otherwise, the parent of a further subprocess gets a SIGTRAP on child exit
				ptrace(PTRACE_DETACH, child, NULL, NULL);
			} else {
				ptrace(PTRACE_SYSCALL, child, NULL, NULL);
			}
		}
	}
	cout << "Completed " << command << endl;
}
