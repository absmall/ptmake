#include "graphviz.h"

using namespace std;

void GraphViz::output(const std::string &filename)
{
	list<Rule *>::iterator i;

	for( i = Rule::rules.begin(); i != Rule::rules.end(); i ++ ) {
		(*i)->print();
	}
}
