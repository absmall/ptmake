#include <exception>
#include <iostream>
#include "find.h"
#include "parse.h"
#include "argpc.h"
#include "build.h"

using namespace std;

bool makefile_specified = false;
extern "C" int yydebug;
std::string makefile;

void set_makefile( std::string file )
{
	makefile_specified = true;
	makefile = file;
}

#ifdef DEBUG
void debug_mode()
{
	yydebug = 1;
}
#endif

int main(int argc, char *argv[])
{
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
#ifdef DEBUG
		options->addOption( ArgpcOption( "debug", 'd', "Enabled.", debug_mode ) );
#endif
		options->parse( &argc, argv );

		if( !makefile_specified ) {
			makefile = find_makefile( );
		}
        for( int i = 1; i < argc; i ++ ) {
            set_target( argv[i] );
        }
        print_targets();
	} catch ( const std::exception &e ) {
		cerr << "make: " << e.what() << endl;
	}
	return 0;
}
