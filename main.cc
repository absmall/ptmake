#include <exception>
#include <iostream>
#include <find.h>
#include <parse.h>

using namespace std;

int main(int argc, char *argv[])
{
	try {
		std::string makefile = find_makefile( );
		parse_makefile( makefile );
	} catch ( const std::exception &e ) {
		cerr << "make: " << e.what() << endl;
	}
	return 0;
}
