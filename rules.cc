#include <string>
#include <list>
#include <iostream>
#include <algorithm>
#include "file.h"
#include "rules.h"
#include "build.h"
#include "exception.h"
#include "subprocess.h"

using namespace std;

std::list<Rule *> Rule::rules;
std::set<std::string> Rule::buildCache;

void Rule::print()
{
	cout << "Rule:" << endl;
	cout << "Targets:" << endl;

	for(list<string>::iterator i = targets.begin();
			i != targets.end();
			i ++)
	{
		cout << (*i);
	}
	cout << endl;
	cout << "Commands:" << endl;
	for(list<string>::iterator i = commands.begin();
			i != commands.end();
			i ++ )
	{
		cout << (*i) << endl;
	}
	cout << endl;
}

void print(std::string filename)
{
	cout << "Depends on " << filename << endl;
}

void Rule::callback(std::string filename)
{
	try {
		try_to_build(filename);
	} catch( wexception &e ) {
		// Do nothing
	}
}

void Rule::execute()
{
	targetTime = fileTimeEarliest( targets );
	for(list<string>::iterator i = commands.begin(); i != commands.end(); i ++ ) {
		trace(*i);
	}
}

Rule::Rule( )
{
	rules.push_back(this);
}

Rule *Rule::find(const string &target)
{
	Rule *r;
	bool foundARule = false;
	for(list<Rule *>::iterator i = rules.begin(); i != rules.end(); i ++ )
	{
		std::list<std::string>::iterator b, e;
		b = (*i)->targets.begin();
		e = (*i)->targets.end();
		if( e != std::find(b, e, target) ) {
			if( foundARule ) {
				// We have multiple rules to build the target
				throw runtime_wexception( "Multiple rules" );
			}
			foundARule = true;
			r = *i;
		}
	}

	if( foundARule ) {
		return r;
	} else {
		throw runtime_wexception( "No rule" );
	}
}

void Rule::try_to_build(const string &target)
{
	// See if we have already built this
	if( buildCache.find(target) != buildCache.end() ) {
		// Already built
		return;
	}

	buildCache.insert(target);
	Rule *r = Rule::find(target);
	r->execute();
}

void Rule::addTarget(const std::string &target)
{
	targets.push_back(target);
}
void Rule::addCommand(const std::string &command)
{
	commands.push_back(command);
}

void Rule::setDefaultTargets(void)
{
	if( !rules.empty() ) {
		Rule *r = *rules.begin();
		for(list<string>::iterator i = r->targets.begin(); i != r->targets.end(); i ++ ) {
			set_target(*i);
		}
	}
}
