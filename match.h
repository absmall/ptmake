#ifndef __MATCH_H__
#define __MATCH_H__

#include <string>

class Match {
public:
    virtual std::string substitute(const std::string &input);
};

#endif /* __MATCH_H__ */
