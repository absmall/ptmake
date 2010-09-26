#include <string>
#include <list>
#include <set>
#include "subprocess.h"

class Rule : public Subprocess {
	public:
		Rule();
		void print();
		void addTarget(const std::string &target);
		void addCommand(const std::string &command);
		void execute();
		static Rule *find(const std::string &target);
		void callback(std::string filename);
		static void setDefaultTargets(void);

	private:
		std::list<std::string> targets;
		std::list<std::string> commands;
		static std::list<Rule *> rules;
};
