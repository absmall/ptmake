#ifndef __MAKE_RULES_H__
#define __MAKE_RULES_H__

#include "rules.h"

class MakeRule : public Rule {
	bool match(const std::string &target);
	std::string expand_command( const std::string &command );
};

#endif /* __MAKE_RULES_H__ */
