#include <string>
#include <list>
#include <iostream>
#include <rules.h>

using namespace std;

struct Dependencies {
	list<string *> *regular;
	list<string *> *orderOnly;
};

struct RuleHeader {
	list<string *> *targetlist;
	Dependencies *dependencies;
};

struct Rule {
	RuleHeader *header;
	list<string *> *commands;
};

void print_rule(void *_rule)
{
	Rule *rule = (Rule *)_rule;

	cout << "Rule:" << endl;
	cout << "Targets:" << endl;
	for(list<string *>::iterator i = rule->header->targetlist->begin();
			i != rule->header->targetlist->end();
			i ++)
	{
		cout << (*(*i));
	}
	cout << endl;
}

void *make_rule(void *_ruleHeader, void *_commands)
{
	RuleHeader *ruleHeader = (RuleHeader *)_ruleHeader;
	list<string *> *commands = (list<string *> *)_commands;

	Rule *rule = new Rule;
	rule->header = ruleHeader;
	rule->commands = commands;

	return rule;
}

void *make_rule_header(void *_targetlist, void *_dependencies)
{
	list<string *> *targetlist = (list<string *> *)_targetlist;
	Dependencies *dependencies = (Dependencies *)_dependencies;

	RuleHeader *ruleHeader = new RuleHeader;
	ruleHeader->targetlist = targetlist;
	ruleHeader->dependencies = dependencies;

	return ruleHeader;
}

void *make_dependencies(void *_regular, void *_orderOnly)
{
	Dependencies *dep = new Dependencies;
	list<string *> *regular = (list<string *> *)_regular;
	list<string *> *orderOnly = (list<string *> *)_orderOnly;

	dep->regular = regular;
	dep->orderOnly = orderOnly;

	return dep;
}

void *new_stringlist()
{
	return new list<string>;
}

void *add_stringlist(void *_slist, void *_ns)
{
	list<string *> *slist = (list<string *> *)_slist;
	string *ns = (string *)_ns;
	slist->push_back(ns);
	return _slist;
}
