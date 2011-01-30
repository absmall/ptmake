#ifndef __RE_H__
#define __RE_H__

#include <string>

/**
 * Basic regular expression syntax. May need to be replaced with something
 * more sophisticated later. We don't want to use straight regular expressions
 * because automatic variables may depend on the target, and I'm not sure
 * how to express those
 */
bool match( const std::string &target, const std::string &file );


#endif
