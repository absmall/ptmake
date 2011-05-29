#include <iostream>
#include <fstream>
#include "file.h"
#include "graphviz.h"
#include "dependencies.h"
#include "utilities.h"

using namespace std;

void GraphViz::output(const std::string &filename)
{
	unsigned char hash[32];
	GraphViz *gv;
	ofstream out(filename.c_str(), ios_base::out);
	
	list<Rule *>::iterator i;
	std::list<std::pair<std::string, bool> > *dependencies;

	out << "digraph G {" << endl;
	memset( hash, 0, 32 );
	for( i = Rule::rules.begin(); i != Rule::rules.end(); i ++ ) {
		gv = (GraphViz *)*i;

		// Print dependencies
		dependencies = retrieve_dependencies( hash );
		if( dependencies != NULL ) {
			for( list<pair<string, bool> >::iterator j = dependencies->begin(); j != dependencies->end(); j ++ ) {
				if( !fileIsAbsolute( j->first ) ) {
					for( list<string>::iterator k = gv->targets->begin(); k != gv->targets->end(); k ++ ) {
						out << "\"" << j->first << "\" -> \"" << *k << "\"" << endl;
					}
				}
			}
			delete dependencies;
		}

	}
	out << "}" << endl;
}
