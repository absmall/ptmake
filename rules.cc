#include <string>
#include <list>
#include <iostream>
#include "rules.h"
#include "build.h"

using namespace std;

void Rule::print()
{
	cout << "Rule:" << endl;
	cout << "Targets:" << endl;

    if( !has_target() ) {
        // There was no target specified, so all targets become default targets
        for(list<string>::iterator i = header->targetlist->begin();
                i != header->targetlist->end();
                i ++)
        {
            set_target(*i);
        }
    }
	for(list<string>::iterator i = header->targetlist->begin();
			i != header->targetlist->end();
			i ++)
	{
		cout << (*i);
	}
	cout << endl;
	cout << "Dependencies:" << endl;
	for(list<string>::iterator i = header->dependencies->regular->begin();
			i != header->dependencies->regular->end();
			i ++)
	{
		cout << (*i);
	}
    cout << endl;
    cout << "Commands:" << endl;
    for(list<string>::iterator i = commands->begin();
            i != commands->end();
            i ++ )
    {
        cout << (*i) << endl;
    }
    cout << endl;
}

Rule::Rule(RuleHeader *ruleHeader, list<string> *commands)
{
	header = ruleHeader;
	commands = commands;
}

RuleHeader *make_rule_header(list<string> *targetlist, Dependencies *dependencies)
{
	RuleHeader *ruleHeader = new RuleHeader;
	ruleHeader->targetlist = targetlist;
	ruleHeader->dependencies = dependencies;

	return ruleHeader;
}

Dependencies *make_dependencies(list<string> *regular, list<string> *orderOnly)
{
	Dependencies *dep = new Dependencies;

	dep->regular = regular;
	dep->orderOnly = orderOnly;

	return dep;
}
