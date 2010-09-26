#include <string>
#include <list>
#include <iostream>
#include "build.h"
#include "rules.h"
#include "exception.h"

using namespace std;

list<string> targets;
set<string> built_cache;

void set_default_target()
{
	if( !has_target() ) {
		Rule::setDefaultTargets();
	}
}

void set_target(string target)
{
	targets.push_back(target);
}

bool has_target()
{
	return !targets.empty();
}

void print_targets()
{
	for(list<string>::iterator i = targets.begin(); i != targets.end(); i ++ ) {
		cout << "Target: " << *i << endl;
	}
}

// For each target, look for a rule that builds the target
void build_targets()
{
	if( targets.empty() ) {
		cerr << "No target specified" << endl;
	}
	for(list<string>::iterator i = targets.begin(); i != targets.end(); i ++ ) {
		try {
			cout << "Making " << *i << " using rule:" << endl;
			Rule::try_to_build( *i );
		} catch (wexception &e) {
			cerr << "Building " << *i << " failed: " << e.what() << endl;
		}
	}
}
