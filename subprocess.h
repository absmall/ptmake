#ifndef __SUBPROCESS_H__
#define __SUBPROCESS_H__

#include <list>
#include <string>

class Subprocess
{
public:
	void trace(std::string command);
	virtual void callback_entry(std::string filename) = 0;
	virtual void callback_exit(std::string filename, bool success) = 0;
};

#endif /* __SUBPROCESS_H__ */
