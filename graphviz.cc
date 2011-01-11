#include <iostream>
#include "graphviz.h"
#include "dependencies.h"
#include "utilities.h"

using namespace std;

void GraphViz::output(const std::string &filename)
{
	list<Rule *>::iterator i;
	std::list<std::pair<std::string, bool> > *dependencies;

	cout << "digraph G {" << endl;
	for( i = Rule::rules.begin(); i != Rule::rules.end(); i ++ ) {

		// Print dependencies
		dependencies = retrieve_dependencies( (*i)->hash );
		if( dependencies != NULL ) {
			for( list<pair<string, bool> >::iterator j = dependencies->begin(); j != dependencies->end(); j ++ ) {
				cout  << "\""<< j->first  << "\""<< " -> " << printhash((*i)->hash) << endl;
			}
			delete dependencies;
		}

		// Print targets
		for( list<string>::iterator j = (*i)->targets->begin(); j != (*i)->targets->end(); j ++ ) {
			cout << printhash((*i)->hash) << " -> " << "\"" << *j << "\"" << endl;
		}
	}
	cout << "}" << endl;
}
