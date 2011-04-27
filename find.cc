#include "find.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdexcept>

extern const char *makefileNames[];
extern const unsigned int makefileNameCount;

static bool test_file_exists( std::string pattern )
{
	struct stat buf;

	return !stat(pattern.c_str(), &buf);

}

std::string find_makefile( void )
{
	for( unsigned int i = 0; i < makefileNameCount; i ++ )
	{
		if( test_file_exists( makefileNames[ i ] ) ) {
			return makefileNames[ i ];
		}
	}

	throw std::runtime_error( "No makefile found" );
}
