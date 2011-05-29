#include <iostream>
#include <fstream>
#include "file.h"
#include "plotter.h"
#include "dependencies.h"
#include "utilities.h"

using namespace std;

Plotter::Plotter()
{
	active = false;
}

Plotter::~Plotter()
{
	if( active ) {
		file << "}" << endl;
		file.close();
	}
}

void Plotter::open(const std::string &filename)
{
	file.open(filename.c_str(), ios_base::out);
	file << "digraph G {" << endl;
	active = true;
}


void Plotter::output(string target, string dep)
{
	if( !fileIsAbsolute( dep ) ) {
		file << "\"" << dep << "\" -> \"" << target << "\"" << endl;
	}
}
