#include <iostream>
#include <fstream>
#include "graphviz.h"
#include "dependencies.h"
#include "utilities.h"

using namespace std;

void GraphViz::output(const std::string &filename)
{
	ofstream out(filename.c_str(), ios_base::out);
	
	list<Rule *>::iterator i;
	std::list<std::pair<std::string, bool> > *dependencies;

	out << "digraph G {" << endl;
	for( i = Rule::rules.begin(); i != Rule::rules.end(); i ++ ) {

		// Print dependencies
		dependencies = retrieve_dependencies( (*i)->hash );
		if( dependencies != NULL ) {
			for( list<pair<string, bool> >::iterator j = dependencies->begin(); j != dependencies->end(); j ++ ) {
				out  << "\""<< j->first  << "\" -> \"" << printhash((*i)->hash) << "\"" << endl;
			}
			delete dependencies;
		}

		// Print targets
		for( list<string>::iterator j = (*i)->targets->begin(); j != (*i)->targets->end(); j ++ ) {
			out << "\"" << printhash((*i)->hash) << "\" -> \"" << *j << "\"" << endl;
		}
	}
	out << "}" << endl;
}
