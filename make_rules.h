#ifndef __MAKE_RULES_H__
#define __MAKE_RULES_H__

#include "rules.h"

class MakeRule : public Rule {
    /* Determine whether a rule matches a target. If it does, return a
     * Match object describing the match.
     */
    bool match( const std::string &target, Match **match );
    std::string expand_command( const std::string &command, const std::string &target, Match *m );
};

#endif /* __MAKE_RULES_H__ */
