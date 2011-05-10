#ifndef __RULES_H__
#define __RULES_H__

#include <string>
#include <list>
#include <set>
#include "subprocess.h"
#include <gcrypt.h>

/* A class which encompasses a rule for building a set of targets using a set
 * of shell commands. \ is used as an escape for special characters. Special
 * characters include * which is used as a wildcard, and {} which are used
 * to reference previous wildcards
 */
class Rule : public Subprocess {
	public:
		Rule();

		/* For debugging purposes, print out everything about the rule */
		void print();

		/* Add a target that the rule will build */
		void addTarget(const std::string &target);

		/* Add multiple targets that the rule will build */
		void addTargetList(std::list<std::string> *targetList);

		/* Add a command to run to perform the build */
		void addCommand(const std::string &command);

		/* Add multiple commands to run in order to perform the build */
		void addCommandList(std::list<std::string> *commandList);

		/* Run the commands to build the targets */
		bool execute(const std::string &target);

		/* Find a rule that matches a target */
		static Rule *find(const std::string &target);

		/* Indicates whether or not a rule matches a target */
		virtual bool match(const std::string &target);

		/* Callback when entering a kernel filesystem call when running the
		 * commands
		 */
		void callback_entry(std::string filename);

		/* Callback when leaving a kernel filesystem call when running the
		 * commands
		 */
		void callback_exit(std::string filename, bool success);

		/*
		 * Check the rules database and set the default target, if no target
		 * was specified
		 */
		static void setDefaultTargets(void);

		/*
		 * Perform variable expansion
		 */
		virtual std::string expand_command( const std::string &command, const std::string &target );

	protected:
		/* Recalculate a hash that describes this rule. It's based on all paramters
		 * that are user-configurable
		 */
		bool built;
		void recalcHash(void);
		unsigned char hash[32];
		time_t targetTime;
		std::list<std::string> *targets;
		std::list<std::string> *commands;
		std::set<std::pair<std::string, bool> > dependencies;
		static std::list<Rule *> rules;
		static std::set<std::string> buildCache;
};

#endif /* __RULES_H__ */
