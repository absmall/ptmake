#include <aprpp.h>
#include <find.h>
#include <globals.h>
#include <apr_tables.h>
#include <apr_fnmatch.h>
#include <stdexcept>

const char *makefileNames[ ] = 
{
	"GNUmakefile",
	"makefile",
	"Makefile"
};

bool test_file_exists( std::string pattern )
{
	AprPP aprpp;
	apr_array_header_t *files;

	apr_match_glob( pattern.c_str( ), &files, aprpp.getPool( ) );

	return !apr_is_empty_array( files );

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
