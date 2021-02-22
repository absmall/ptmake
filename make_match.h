#ifndef __MAKE_MATCH__
#define __MAKE_MATCH__

#include <stddef.h>
#include <string>

#include "match.h"

class MakeMatch : public Match {
public:
    bool initialize(const std::string &target, const std::string &pattern);
    std::string substitute(const std::string &input);
private:
    std::string target;
    std::string pattern;
    size_t wildcard_t;
    bool wildcard;
};

#endif /* __MAKE_MATCH__ */
