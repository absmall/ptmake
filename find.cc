#include <aprpp.h>
#include <find.h>
#include <globals.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdexcept>

const char *makefileNames[ ] = 
{
	"GNUmakefile",
	"makefile",
	"Makefile"
};

bool test_file_exists( std::string pattern )
{
	struct stat buf;

	return !stat(pattern.c_str(), &buf);

}

std::string find_makefile( void )
{
	for( int i = 0; i < sizeof( makefileNames ) / sizeof( char * ); i ++ )
	{
		if( test_file_exists( makefileNames[ i ] ) ) {
			return makefileNames[ i ];
		}
	}

	throw std::runtime_error( "No makefile found" );
}
