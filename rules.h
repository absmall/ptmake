#include <string>

struct Dependencies {
	std::list<std::string> *regular;
	std::list<std::string> *orderOnly;
};

struct RuleHeader {
	std::list<std::string> *targetlist;
	Dependencies *dependencies;
};

class Rule {
public:
    Rule(RuleHeader *ruleHeader, std::list<std::string> *commands);
    void print();

private:
	RuleHeader *header;
	std::list<std::string> *commands;
};

RuleHeader *make_rule_header(std::list<std::string> *, Dependencies *);
Dependencies *make_dependencies(std::list<std::string> *, std::list<std::string> *);
