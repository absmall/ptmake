#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "file.h"
#include "exception.h"
#include <iostream>
#include <errno.h>
#include <string.h>
#include <string>

using namespace std;

bool fileTime(const string &file, time_t &time, bool *isDir)
{
    struct stat s;

    if( stat( file.c_str(), &s ) ) {
        return false;
    }

    time = s.st_mtime;
    *isDir = S_ISDIR(s.st_mode);
    return true;
}

time_t fileTime(const string &file)
{
    struct stat s;

    if( stat( file.c_str(), &s ) ) {
        throw runtime_wexception( "Cannot access file time" );
    }

    return s.st_mtime;
}

time_t fileTimeEarliest(const list<string> &files)
{
    list<string>::const_iterator i;
    time_t earliest, t;

    if( files.empty() ) {
        throw runtime_wexception( "Retrieving timestamp with no files" );
    }

    i = files.begin();

    earliest = fileTime( *i );

    while( i != files.end() ) {
        t = fileTime( *i );
        if( t < earliest ) earliest = t;
        i ++;
    }

    return earliest;
}

bool fileIsAbsolute( const string &file )
{
    return file[ 0 ] == '/';
}

bool fileExists(const string &file)
{
    struct stat s;

    return !stat( file.c_str(), &s );
}

#if 1
// Realpath seems to be incredibly slow (on linux, x86)
// I guess it's because of resolving symlinks? I don't care TOO much about
// symlinks (though it would be cool to support them, but we can't support
// hardlinks anyway, and probably not mount points, so probably not worth it)
// so I'd like to replace them with the custom code below that doesn't handle
// symlinks once that code is working
string fileCanonicalize( string path )
{
    char buf[ PATH_MAX ];
    char *ret;

    ret = realpath( path.c_str(), buf );

    if( ret == NULL ) {
        if( errno == ENOENT ) {
            // File was not found, so canonicalization doesn't really matter
            return path;
        } else {
            throw runtime_wexception( string( "Error canonicalizing path '" ) + path + string("'") );
        }
    }
    return buf;
}
#else
string fileCanonicalize( string path )
{
    int i, length, start=0;
    char buf[ PATH_MAX ];
    string ret;
    enum {
        COPY,
        DOT,
        DOTDOT
    } state=COPY;

    if( path.length() == 0 ) return path;

    // See if the path is absolute
    if( path[0] != '/' ) {
        ret = string( getcwd( buf, PATH_MAX ) ) + "/";
    }

    length = path.length();
    for(i = 0; i < length; i ++ ) {
        switch( path[ i ] ) {
            case '.':
                switch( state ) {
                    case COPY:
                        state = DOT;
                        break;
                    case DOT:
                        state = DOTDOT;
                        break;
                    case DOTDOT:
                        state = COPY;
                        break;
                }
                break;
            case '/':
                switch( state ) {
                    case COPY:
                        // Just keep copying
                        break;
                    case DOT:
                        // We have a dot, just remove it
                        cout << ". with " << ret << start << "," << i-2 << ")" << endl;
                        ret += path.substr( start, i-2 );
                        cout << "Killing " << ret << " for ." << endl;
                        start = i;
                        state = COPY;
                        break;
                    case DOTDOT:
                        // We have a dotdot, copy over, then remove the last thing before it
                        cout << ".. with " << ret << "(" << start << "," << i-3 << ")" << endl;
                        ret += path.substr( start, i-3 );
                        cout << "Killing with " << ret << endl;
                        ret.erase(ret.find_last_of('/'));
                        start = i;
                        state = COPY;
                        break;
                }
            default:
                // Keep copying
                break;
        }
    }
    
    return ret + path.substr( start );
}
#endif
