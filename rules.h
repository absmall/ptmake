#include <string>
#include <list>
#include <set>
#include "subprocess.h"
#include <gcrypt.h>

class Rule : public Subprocess {
	public:
		Rule();
		void print();
		void addTarget(const std::string &target);
		void addTargetList(std::list<std::string> *targetList);
		void addCommand(const std::string &command);
		void addCommandList(std::list<std::string> *commandList);
		bool execute();
		static Rule *find(const std::string &target);
		void callback_entry(std::string filename);
		void callback_exit(std::string filename, bool success);
		static void setDefaultTargets(void);

	private:
		/* Recalculate a hash that describes this rule. It's based on all paramters
		 * that are user-configurable
		 */
		bool built;
		void recalcHash(void);
		unsigned char hash[32];
		time_t targetTime;
		std::list<std::string> *targets;
		std::list<std::string> *commands;
		std::list<std::pair<std::string, bool> > dependencies;
		static std::list<Rule *> rules;
		static std::set<std::string> buildCache;
};
