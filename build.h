#ifndef __BUILD_H__
#define __BUILD_H__

#include <string>
#include <set>

/*
 * Set the default target. If there is no target set, it will be the target of
 * the first rule.
 */
void set_default_target();
bool has_target();
void set_target(std::string target);
void print_targets();
void build_targets();
extern std::set<std::string> built_cache;

#endif /* __BUILD_H__ */
