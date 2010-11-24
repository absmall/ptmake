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
		void execute();
		static Rule *find(const std::string &target);
		void try_to_build(const std::string &target, bool exists);
		void callback(std::string filename, bool success);
		static void setDefaultTargets(void);

	private:
		/* Recalculate a hash that describes this rule. It's based on all paramters
		 * that are user-configurable
		 */
		void recalcHash(void);
		unsigned char hash[32];
		time_t targetTime;
		std::list<std::string> *targets;
		std::list<std::string> *commands;
		static std::list<Rule *> rules;
		static std::set<std::string> buildCache;
};
