#include "exception.h"

runtime_wexception::runtime_wexception( const std::string &s ) : s( s )
{
}

runtime_wexception::~runtime_wexception( )
{
}

const char *runtime_wexception::what( void ) const throw( )
{
	return s.c_str( );
}
