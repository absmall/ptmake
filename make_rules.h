#ifndef __MAKE_RULES_H__
#define __MAKE_RULES_H__

#include "rules.h"

class MakeRule : public Rule {
    bool match( const std::string &target, Match **match );
    /* Retrieve the string which would be the dependency corresponding to a
     * target. This may differ from 'dep' in the case of a pattern rule
     */
    bool getDepName( const std::string &target, const std::string &declTarget, const std::string &declDep, std::string &ret );
    std::string expand_command( const std::string &command, const std::string &target, Match *m );
};

#endif /* __MAKE_RULES_H__ */
