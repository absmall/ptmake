#include <string>
#include <list>
#include <iostream>
#include "build.h"

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
