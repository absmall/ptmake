#include <exception>
#include <iostream>
#include "find.h"
#include "parse.h"
#include "argpc.h"
#include "build.h"
#include "graphviz.h"

using namespace std;

bool makefile_specified = false;
bool debug = false;
string plotfile = "";
extern int yydebug;
std::string makefile;
extern std::string depfile;

void set_makefile( std::string file )
{
	makefile_specified = true;
	makefile = file;
}

void set_depfile( std::string file )
{
	depfile = file;
}

void plot( std::string file )
{
	plotfile = file;
}

#ifdef DEBUG
void debug_mode()
{
	yydebug = 1;
	debug = 1;
}
#endif

int main(int argc, char *argv[])
{
	int ret;
	try {
		Argpc *options = Argpc::getInstance( );

		// Set up the standard argument options
		options->addHelp( );
		options->addUsage( );
		options->addVersion( );
		options->setVersion( "0.1" );
		options->setBugAddress( "Aaron Small <a.small@unb.ca>" );
		options->setDoc( "Yet another make clone" );
		options->setArgumentDescription( "FILE ..." );
		options->addOption( ArgpcOption( "file", 'f', "file", "Read FILE as a makefile.", set_makefile ) );
		options->addOption( ArgpcOption( "depfile", 'b', "depfile", "Use specified file as dependency database.", set_depfile ) );
		options->addOption( ArgpcOption( "plot", 'p', "graphfile", "Draw the cached dependency information", plot ) );
#ifdef DEBUG
		options->addOption( ArgpcOption( "debug", 'd', "Ouput debugging information", debug_mode ) );
#endif
		options->parse( &argc, argv );

		if( !makefile_specified ) {
			makefile = find_makefile( );
		}
		parse_makefile( makefile );
		for( int i = 1; i < argc; i ++ ) {
			set_target( argv[i] );
		}
		set_default_target();
		ret = build_targets();
		if( plotfile != "" ) {
			GraphViz g;
			g.output( plotfile );
		}
	} catch ( const std::exception &e ) {
		cerr << "make: " << e.what() << endl;
		return 1;
	}
	return ret;
}
