#ifndef __FILE_H__
#define __FILE_H__

#include <list>
#include <string>
#include <time.h>

/*
 * Return the file access time for a specified file. If the file doesn't exist,
 * throw an exception
 */
time_t fileTime(const std::string &file);

/*
 * Return the file access time for a specified file. Return value indicates
 * whether or not the file exists
 */
bool fileTime(const std::string &file, time_t &time);

/*
 * Return the earliest file access time for any of a set of files
 */
time_t fileTimeEarliest(const std::list<std::string> &files);

#endif /* __FILE_H__ */
