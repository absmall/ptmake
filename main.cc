#include <exception>
#include <iostream>
#include <find.h>
#include <parse.h>
#include <argpc.h>

using namespace std;

bool makefile_specified = false;
std::string makefile;

void set_makefile( std::string file )
{
	makefile_specified = true;
	makefile = file;
}

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
		options->parse( &argc, argv );

		if( !makefile_specified ) {
			makefile = find_makefile( );
		}
		parse_makefile( makefile );

		if( argc > 1 ) {
			for( int i = 0; i < argc; i ++ ) {
				cout << "Using target " << argv[i] << endl;
			}
		} else {
			cout << "Using first target " << endl;
		}
	} catch ( const std::exception &e ) {
		cerr << "make: " << e.what() << endl;
	}
	return 0;
}
