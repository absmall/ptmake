#ifndef __BUILD_H__
#define __BUILD_H__

#include <string>

/*
 * Set the default target. If there is no target set, it will be the target of
 * the first rule.
 */
void set_default_target();

/*
 * Sets a specified target that the user wants to build.
 */
void set_target(std::string target);

/*
 * Build all the targets that have been specified to this point
 */
int build_targets();

#endif /* __BUILD_H__ */
