#ifndef __MAKE_MATCH__
#define __MAKE_MATCH__

#include "match.h"
#include <string>

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
