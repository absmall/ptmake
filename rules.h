#include <string>
#include <list>
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

	private:
		std::list<std::string> targets;
		std::list<std::string> commands;
		static std::list<Rule *> rules;
};
