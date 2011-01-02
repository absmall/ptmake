#ifndef __DEPENDENCIES_H__
#define __DEPENDENCIES_H__

#include <list>
#include <string>

void clear_dependencies(unsigned char hash[32]);
void add_dependencies(unsigned char hash[32], std::string dep, bool success);
void add_dependencies(unsigned char hash[32], const std::set<std::pair<std::string, bool> > &deps);
std::list<std::pair<std::string, bool> > *retrieve_dependencies(unsigned char hash[32]);

#endif /* __DEPENDENCIES_H__ */
