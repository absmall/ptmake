#include "variables.h"
#include "exception.h"

using namespace std;

class RegularVariable
{
	void Set( std::string s )
	{
		value = s;
	}

	std::string Get( )
	{
		return value;
	}

	std::string value;
};

Scope::~Scope()
{
	map<string, Variable *>::iterator i;

	// Destroy all variables when exiting a scope
	for(i = variables.begin(); i != variables.end(); i ++ )
	{
		delete i->second;
	}
}

string Scope::lookup( string name )
{
	map<string, Variable *>::iterator i;

	i = variables.find( name );
	if( i == variables.end() ) {
		throw runtime_wexception("Variable not defined");
	}

	return i->second->Get( );
}
