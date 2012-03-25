#ifndef __FILE_H__
#define __FILE_H__

#include <list>
#include <string>
#include <time.h>

/*
 * Return the file access time for a specified file. If the file doesn't exist,
 * throw an exception
 */
int fileTime(const std::string &file, time_t *time);

/*
 * Return the file access time for a specified file. Return value indicates
 * whether or not the file exists
 */
int fileTime(const std::string &file, time_t *time, bool *isDir);

/*
 * Return the earliest file access time for any of a set of files
 */
int fileTimeEarliest(const std::list<std::string> &files, time_t *time);

/*
 * Return whether or not a path is an absolute path
 */
bool fileIsAbsolute( const std::string &file );

/*
 * Return whether the specified file exists.
 */
bool fileExists(const std::string &file);

/*
 * Return the absolute path to a file from a path that may
 * be absolute or relative
 */
std::string fileCanonicalize( std::string path );

#endif /* __FILE_H__ */
