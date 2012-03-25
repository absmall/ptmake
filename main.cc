#include <exception>
#include <iostream>
#include "find.h"
#include "parse.h"
#include "argpc.h"
#include "build.h"
#include "plotter.h"
#include "debug.h"
#include "dependencies.h"

using namespace std;

bool makefile_specified = false;
string plotfile = "";
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

int main(int argc, char *argv[])
{
    Plotter p;
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
        options->setArgumentDescription( "..." );
        options->addOption( ArgpcOption( "file", 'f', "file", "Read FILE as a makefile.", set_makefile ) );
        options->addOption( ArgpcOption( "depfile", 'b', "depfile", "Use specified file as dependency database.", set_depfile ) );
        options->addOption( ArgpcOption( "plot", 'p', "graphfile", "Draw the cached dependency information", plot ) );

        debug_init( );

        options->parse( &argc, argv );

        dependencies_init();

        if( !makefile_specified ) {
            makefile = find_makefile( );
        }
        parse_makefile( makefile );
        for( int i = 1; i < argc; i ++ ) {
            set_target( argv[i] );
        }
        set_default_target();
        if( plotfile != "" ) {
            p.open( plotfile );
            set_plotter( &p );
        }
        ret = build_targets();
    } catch ( const std::exception &e ) {
        cerr << "make: " << e.what() << endl;
        dependencies_deinit();
        return 1;
    }
    dependencies_deinit();
    return ret;
}
