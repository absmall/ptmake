#ifndef __SUBPROCESS_H__
#define __SUBPROCESS_H__

#include <list>
#include <string>

void trace(std::string command, void (*callback)(std::string filename));

#endif /* __SUBPROCESS_H__ */
