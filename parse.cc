#include <parse.h>
#include <aprpp.h>
#include <iostream>

void parse_makefile( std::string filename )
{
	std::string line;
	AprFile in( filename, APR_READ );
	while( !in.eof( ) ) {
		line = in.readLine( );
		// Ignore blank lines
		if( line.length() == 0 ) continue;
		std::cout << "Line: " << line << std::endl;
	}
}
