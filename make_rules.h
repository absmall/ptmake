#ifndef __MAKE_RULES_H__
#define __MAKE_RULES_H__

#include "rules.h"

class MakeRule : public Rule {
	bool match(const std::string &target);
};

#endif /* __MAKE_RULES_H__ */
