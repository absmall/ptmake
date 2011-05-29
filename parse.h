#ifndef __PARSE_H__
#define __PARSE_H__

#include <string>

// Commands related to a particular build system (make, jam, etc.)

// Read in the file specified
void parse_makefile( std::string filename );

// Expand a command according to the system's variable expansion rules
std::string expand_command( std::string command );

#endif /* __PARSE_H__ */
