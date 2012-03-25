#include <map>
#include <string>
#include <sys/ptrace.h>
#include <iostream>
#include "subprocess.h"
#include "debug.h"

using namespace std;

#ifdef DEBUG
struct
{
    int id;
    const char *statement;
} static debugprints[] = {
    { 0, "something" },
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
        ptrace(PT_TRACE_ME, 0, NULL, NULL);
        execl("/bin/sh", "sh", "-c", command.c_str(), NULL);
    } else {
        while(1) {
            child = wait(&status);
            //ptrace(PTRACE_SETOPTIONS, child, NULL, PTRACE_O_TRACEFORK | PTRACE_O_TRACEVFORK | PTRACE_O_TRACECLONE | PTRACE_O_TRACEEXEC);
            if(WIFEXITED(status)) break;
            // Get syscall_id
#ifdef DEBUG
            if( get_debug_level( DEBUG_SUBPROCESS ) ) {
                debugprint( child, syscall_id, returnVal );
            }
#endif
            switch( syscall_id ) {
                case 0:
#if 0
                case __NR_stat:
                case __NR_access:
                case __NR_open:
#if defined(__i386__)
                case __NR_stat64:
#endif
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
#if 0
                    if( syscall_id == __NR_access ) {
                        returnVal = ptrace(PTRACE_PEEKUSER, child, ARG2, NULL);
                        if( returnVal == W_OK ) break;
                    } else if( syscall_id == __NR_open ) {
                        returnVal = ptrace(PTRACE_PEEKUSER, child, ARG2, NULL);
                        if( returnVal & O_CREAT ) break;
                    }
#endif

                    // The first argument for all of these 
                    //returnVal = ptrace(PTRACE_PEEKUSER, child, ARG1, NULL);
                    done = 0;
#if 0
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
#endif

                    // There should be a more elegant way to do this - we want to exclude
                    // proc and sys because they contain files whose timestamps constantly
                    // increment, and we exclude tmp because tools may write then read
                    // temporary files there, and we don't want to depend on those.
                    if( !strncmp(s.c_str(), "/proc", 5 ) ) break;
                    if( !strncmp(s.c_str(), "/sys", 4 ) ) break;
                    if( !strncmp(s.c_str(), "/tmp", 4 ) ) break;
                    if( !strncmp(s.c_str(), ".", 2 ) ) break;

                    if( insyscall ) {
                        callback_entry(s);
                    } else {
                        //status = ptrace(PTRACE_PEEKUSER, child, RETURNVAL, NULL);
                        status = 0;
                        callback_exit(s, status >= 0);
                    }
                }
            }
            if( syscall_id == 0 ) {
            //if( syscall_id == __NR_exit_group ) {
                // Detach here - otherwise, the parent of a further subprocess gets a SIGTRAP on child exit
                insyscallMap.erase(child);
                ptrace(PT_DETACH, child, NULL, NULL);
            } else {
                // Go on to the next
                //ptrace(PT_SYSCALL, child, NULL, NULL);
            }
        }
    }

    if( get_debug_level( DEBUG_SUBPROCESS ) ) {
        cout << "Completed " << command << endl;
    }
}
