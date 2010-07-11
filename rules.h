#include <string>
#include <list>

class Rule {
public:
    Rule();
    void print();
    void addTarget(const std::string &target);
    void addCommand(const std::string &command);
    static Rule *find(const std::string &target);

private:
    std::list<std::string> targets;
	std::list<std::string> commands;
    static std::list<Rule *> rules;
};
