#include <string>
#include <list>
#include <iostream>
#include "build.h"
#include "rules.h"
#include "exception.h"

using namespace std;

list<string> targets;

static bool has_target()
{
    return !targets.empty();
}

void set_default_target()
{
    if( !has_target() ) {
        Rule::setDefaultTargets();
    }
}

void set_plotter(Plotter *p)
{
    Rule::setPlotter(p);
}

void set_target(string target)
{
    targets.push_back(target);
}

// For each target, look for a rule that builds the target
int build_targets()
{
    int ret = 0;
    if( targets.empty() ) {
        cerr << "No target specified" << endl;
    }
    for(list<string>::iterator i = targets.begin(); i != targets.end(); i ++ ) {
        try {
            if( !Rule::build( *i ) ) {
                cout << "ptmake: `" << *i << "' is up to date." << endl;
            }
        } catch (wexception &e) {
            cerr << "Building " << *i << " failed: " << e.what() << endl;
            ret = 1;
        }
    }
    return ret;
}
