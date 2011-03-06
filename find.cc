#include "find.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdexcept>

#if defined(MAKEFILE)
const char *makefileNames[ ] = 
{
	"GNUmakefile",
	"makefile",
	"Makefile"
};
#elif defined(JAMFILE)
const char *makefileNames[ ] = 
{
	"Jamfile"
};
#else
#error must specify a file type
#endif

static bool test_file_exists( std::string pattern )
{
	struct stat buf;

	return !stat(pattern.c_str(), &buf);

}

std::string find_makefile( void )
{
	for( unsigned int i = 0; i < sizeof( makefileNames ) / sizeof( char * ); i ++ )
	{
		if( test_file_exists( makefileNames[ i ] ) ) {
			return makefileNames[ i ];
		}
	}

#if defined(MAKEFILE)
	throw std::runtime_error( "No makefile found" );
#elif defined(JAMFILE)
	throw std::runtime_error( "No jamfile found" );
#else
#error must specify a file type
#endif
}
