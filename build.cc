#include <string>
#include <list>
#include <iostream>
#include "build.h"
#include "rules.h"
#include "exception.h"

using namespace std;

list<string> targets;

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
    for(list<string>::iterator i = targets.begin(); i != targets.end(); i ++ ) {
        try {
            Rule *r = Rule::find(*i);
            cout << "Making " << *i << " using rule:" << endl;
            r->print();
        } catch (wexception &e) {
            cerr << "Building " << *i << " failed: " << e.what() << endl;
        }
    }
}
