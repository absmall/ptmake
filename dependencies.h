#ifndef __DEPENDENCIES_H__
#define __DEPENDENCIES_H__

#include <list>
#include <string>

/*
 * This module caches dependencies in a Berkeley DB
 */

/*
 * Open the database
 */
void dependencies_init();

/*
 * Close the database
 */
void dependencies_deinit();

/* Clear all the dependencies associated with a particular rule */
void clear_dependencies(unsigned char hash[32]);

/* Add a set of dependencies associated with a particular rule */
void add_dependencies(unsigned char hash[32], const std::set<std::pair<std::string, bool> > &deps);

/* Retrieve the dependenices for a rule from the database */
std::list<std::pair<std::string, bool> > *retrieve_dependencies(unsigned char hash[32]);

#endif /* __DEPENDENCIES_H__ */
