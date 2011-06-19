#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/reg.h>
#include <sys/ptrace.h>
#include <asm/ptrace-abi.h>
#include <sys/wait.h>
#include <syscall.h>
#include "subprocess.h"
#include "debug.h"

using namespace std;

struct
{
	int id;
	const char *statement;
} static debugprints[] = {
	{ __NR_stat, "stat" },
	{ __NR_access, "access" },
	{ __NR_open, "open" },
	{ __NR_set_thread_area, "set_thread_area" },
	{ __NR_fstat, "fstat" },
	{ __NR_fcntl, "fcntl" },
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
#if defined(__x86_64)
	{ __NR_arch_prctl, "arch prctl" },
#endif
#if defined(__i386__)
	{ __NR_geteuid32, "geteuid32" },
	{ __NR_stat64, "stat64" },
	{ __NR_fstat64, "fstat64" },
	{ __NR_mmap2, "mmap2" },
#endif
};

void debugprint( int pid, long syscall_id, int returnVal )
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

#if defined(__i386)
#define RETURNVAL (4 * EAX)
#define ARG1 (4 * EBX)
#define ARG2 (4 * ECX)
#define ARG3 (4 * EDX)
#elif defined(__x86_64)
#define RETURNVAL (8 * RAX)
#define ARG1 (8 * RDI)
#define ARG2 (8 * RSI)
#define ARG3 (8 * RDX)
#endif

void Subprocess::trace(string command)
{
	char l;
	int i, status;
	long syscall_id, c, returnVal;
	pid_t child;
	bool insyscall;
	map<pid_t,bool> insyscallMap;

	if( get_debug_level( DEBUG_SUBPROCESS ) ) {
		cout << "Executing ";
	}
	cout << command << endl;

	child = fork();
	if( child == 0 ) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl("/bin/sh", "sh", "-c", command.c_str(), (char *)NULL);
	} else {
		while(1) {
			child = wait(&status);
			ptrace(PTRACE_SETOPTIONS, child, NULL, PTRACE_O_TRACEFORK | PTRACE_O_TRACEVFORK | PTRACE_O_TRACECLONE | PTRACE_O_TRACEEXEC);
			if(WIFEXITED(status)) break;

			// Make sure we're being called for a trap
			if(!WIFSTOPPED(status) || WSTOPSIG(status) != SIGTRAP) {
				ptrace(PTRACE_SYSCALL, child, NULL, NULL);
				continue;
			}
#if defined(__i386)
			syscall_id = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
#elif defined(__x86_64)
			syscall_id = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
#endif
#ifdef DEBUG
			returnVal = ptrace(PTRACE_PEEKUSER, child, RETURNVAL, NULL);
			if( get_debug_level( DEBUG_SUBPROCESS ) ) {
				debugprint( child, syscall_id, returnVal );
			}
#endif
			switch( syscall_id ) {
				case __NR_stat:
				case __NR_access:
				case __NR_open:
#if defined(__i386__)
				case __NR_stat64:
#endif
				{
					int done;
					string s;
					map<pid_t,bool>::iterator it = insyscallMap.find(child);

					if( it != insyscallMap.end() ) {
						it->second ^= 1;
						insyscall = it->second;
					} else {
						// New process
						insyscallMap[ child ] = true;
						insyscall = true;
					}
					
					// See if this is being accessed for write. If it is,
					// that's not a dependency
					if( syscall_id == __NR_access ) {
						returnVal = ptrace(PTRACE_PEEKUSER, child, ARG2, NULL);
						if( returnVal == W_OK ) break;
					} else if( syscall_id == __NR_open ) {
						returnVal = ptrace(PTRACE_PEEKUSER, child, ARG2, NULL);
						if( returnVal & O_CREAT ) break;
					}

					// The first argument for all of these 
					returnVal = ptrace(PTRACE_PEEKUSER, child, ARG1, NULL);
					done = 0;
					while( !done ) {
						c = ptrace(PTRACE_PEEKDATA, child, returnVal, NULL);
						for( i = 0; i < 4; i ++ ) {
							l = c & 0xFF;
							c >>= 8;
							returnVal ++;
							if( l == 0 ) {
								done = 1;
								break;
							}
							s += l;
						}
					}

					// There should be a more elegant way to do this - we want to exclude
					// proc and sys because they contain files whose timestamps constantly
					// increment, and we exclude tmp because tools may write then read
					// temporary files there, and we don't want to depend on those.
					if( !strncmp(s.c_str(), "/proc", 5 ) ) break;
					if( !strncmp(s.c_str(), "/sys", 4 ) ) break;
					if( !strncmp(s.c_str(), "/tmp", 4 ) ) break;
					if( !strncmp(s.c_str(), ".", 2 ) ) break;

					returnVal = ptrace(PTRACE_PEEKUSER, child, RETURNVAL, NULL);
					if( insyscall ) {
						callback_entry(s);
					} else {
						callback_exit(s, returnVal >= 0);
					}
				}
			}
			if( syscall_id == __NR_exit_group ) {
				// Detach here - otherwise, the parent of a further subprocess gets a SIGTRAP on child exit
				insyscallMap.erase(child);
				ptrace(PTRACE_DETACH, child, NULL, NULL);
			} else {
				ptrace(PTRACE_SYSCALL, child, NULL, NULL);
			}
		}
	}

	if( get_debug_level( DEBUG_SUBPROCESS ) ) {
		cout << "Completed " << command << endl;
	}
}
