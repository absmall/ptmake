#include <wchar.h>
#include "argpc.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <algorithm>

using namespace std;

Argpc Argpc::instance;

Argpc::Argpc( void )
{
	hasHelp = false;
	hasUsage = false;
}

Argpc::~Argpc( void )
{
}

void Argpc::addOption( const ArgpcOption &o ) {
	options.push_back( o );
}

void Argpc::addOption( basic_string<char> group, const ArgpcOption &o ) {
	options.push_back( o );
	options.back().group = new ArgpcGroup( group, o.group );
}

void Argpc::addHelp( void )
{
	hasHelp = true;
	options.push_back( ArgpcOption( "help", 'h', "Give this help list", printHelp ) );
}

void Argpc::addUsage( void )
{
	hasUsage = true;
	options.push_back( ArgpcOption( "usage", 0, "Give a short usage message", printUsage ) );
}

void Argpc::addVersion( void )
{
	options.push_back( ArgpcOption( "version", 'v', "Display the current version and exit", printVersion ) );
}

void Argpc::parsePartial( int *argc, char *argv[] )
{
	progName = argv[ 0 ];
	try
	{
		for( int i = 1; i < *argc; )
		{
			try
			{
				// See if it's an option
				if( argv[ i ][ 0 ] == '-' )
				{
					// See if it's a long option
					if( argv[ i ][ 1 ] == 0 )
					{
						// It's a -
						// This is actually an argument not an option
						i ++;
						continue;
					} else if( argv[ i ][ 1 ] == '-' ) {
						// It's a long option
						if( argv[ i ][ 2 ] == 0 )
						{
							// It's a --
							// Erase it and Stop parsing options
							removeArg( argc, argv, i );
							return;
						} else {
							// True long option. Parse it.
							parseLongOption( argc, argv, i );
						}
					} else {
						// It's a short option
						parseShortOption( argc, argv, i );
					}
				} else {
					i ++;
				}
			} catch( UnknownArgumentException &e ) {
				// In a partial parse, ignore unknown arguments
				i ++;
			}
		}

	} catch( wexception &e ) {
		displayArgumentError( e.what( ) );
	}
}

void Argpc::setVersion( std::basic_string<char> s )
{
	version = s;
}

void Argpc::parse( int *argc, char *argv[] )
{
	progName = argv[ 0 ];
	try
	{
		for( int i = 1; i < *argc; )
		{
			// See if it's an option
			if( argv[ i ][ 0 ] == '-' )
			{
				// See if it's a long option
				if( argv[ i ][ 1 ] == 0 )
				{
					// It's a -
					// This is actually an argument not an option
					i ++;
					continue;
				} else if( argv[ i ][ 1 ] == '-' ) {
					// It's a long option
					if( argv[ i ][ 2 ] == 0 )
					{
						// It's a --
						// Erase it and Stop parsing options
						removeArg( argc, argv, i );
						return;
					} else {
						// True long option. Parse it.
						parseLongOption( argc, argv, i );
					}
				} else {
					// It's a short option
					parseShortOption( argc, argv, i );
				}
			} else {
				i ++;
			}
		}

	} catch( wexception &e ) {
		displayArgumentError( e.what( ) );
	}
}

void Argpc::setDoc( basic_string<char> s )
{
	doc = s;
}

void Argpc::setBugAddress( basic_string<char> s )
{
	bugAddress = s;
}

void Argpc::setArgumentDescription( basic_string<char> s )
{
	argumentDescription = s;
}

void Argpc::displayArgumentError( basic_string<char> s )
{
	cout << "Error: " << s << endl;
	if( hasHelp && hasUsage )
	{
		cout << "Try `" << progName << " --help' or `" << progName << " --usage' for more information" << endl;
	} else if( hasHelp ) {
		cout << "Try `" << progName << " --help' for more information" << endl;
	} else if( hasUsage ) {
		cout << "Try `" << progName << " --usage' for more information" << endl;
	}
	exit( 0 );
}

Argpc *Argpc::getInstance( void )
{
	return &instance;
}

void Argpc::removeArg( int *argc, char *argv[], int i )
{
	memmove( argv + i, argv + i + 1, (*argc - i - 1) * sizeof( char * ) );
	(*argc) --;
}

void Argpc::parseShortOption( int *argc, char *argv[], int i )
{
	bool match = false;
	bool hasValue = false;
	char flag;
	basic_string<char> value;
	vector< ArgpcOption >::iterator callback;

	flag = argv[ i ][ 1 ];

	for( callback = options.begin( ); callback != options.end( ); callback ++ )
	{
		if( callback->id == flag )
		{
			match = true;
			if( callback->hasValue ) {
				if( argv[ i ][ 2 ] == 0 )
				{
					hasValue = true;
					// The argument is in the next parameter
					if( i + 1 == *argc )
					{
						throw MissingArgumentException( basic_string<char>( &flag, 1 ) );
					}
					value = argv[ i + 1 ];
				} else if( argv[ i ][ 2 ] == '=' ) {
					// The argument follows the =
					value = basic_string<char>( argv[ i ] + 3 );
				} else {
					// The argument follows immediately
					value = basic_string<char>( argv[ i ] + 2 );
				}
				validateAndCallback( callback, value );
			} else {
				callback->callback.noarg( );
			}
		}
	}

	if( match )
	{
		removeArg( argc, argv, i );
		if( hasValue )
		{
			removeArg( argc, argv, i );
		}
	} else {
		// Didn't find a match
		throw UnknownArgumentException( basic_string<char>( &flag, 1 ) );
	}
}

void Argpc::parseLongOption( int *argc, char *argv[], int i )
{
	bool match = false;
	bool hasValue = false;
	int length;
	char *equals = strchr( argv[ i ], L'=' );
	basic_string<char> name;

	// Skip over --
	argv[ i ] += 2;

	if( equals )
	{
		length = equals - argv[ i ];
		equals ++;
	} else {
		length = strlen( argv[ i ] );
	}
	name = basic_string<char>( argv[ i ], length );

	for( vector< ArgpcOption >::iterator arg = options.begin( ); arg != options.end( ); arg ++ )
	{
		if( arg->name == name )
		{
			match = true;
			if( arg->hasValue )
			{
				if( equals ) {
					validateAndCallback( arg, equals );
				} else {
					hasValue = true;
					if( i + 1 == *argc )
					{
						throw MissingArgumentException( argv[ i ] );
					}
					validateAndCallback( arg, argv[ i + 1 ] );
				}
			} else {
				arg->callback.noarg( );
			}
		}

	}

	if( match )
	{
		removeArg( argc, argv, i );
		if( hasValue )
		{
			removeArg( argc, argv, i );
		}
	} else {
		// Didn't find a match
		argv[ i ] -= 2;
		throw UnknownArgumentException( name );
	}
}

void Argpc::validateAndCallback( vector< ArgpcOption >::iterator i, const string &s )
{
	if( i->values.size() == 0 || find(i->values.begin(), i->values.end(), s) != i->values.end() ) {
		i->callback.arg( s );
	} else {
		throw BadParameterException(basic_string<char>(i->name), s);
	}
}

void Argpc::printHelp( void )
{
	basic_string<char> lastgroup;
	cout << "Usage: " << instance.progName << " [OPTION...] " << instance.argumentDescription << endl;
	cout << instance.progName;
	if( instance.version != "" )
	{
		cout << " v" << instance.version;
	}
	if( instance.doc != "" ) {
		cout << " --- " << instance.doc;
	}
	cout << endl << endl;
	sort( instance.options.begin( ), instance.options.end( ) );
	for( vector< ArgpcOption >::iterator i = instance.options.begin( ); i != instance.options.end( ); i ++ )
	{
		basic_string<char> s;

		// See if we need to print a group label
		if( i->group != NULL && i->group->fullName( ) != lastgroup )
		{
			lastgroup = i->group->fullName( );
			cout << endl << lastgroup << " options:" << endl;
		}
		
		// Always indent 2 characters
		s = "  ";

		// If there's a short option, write it
		if( i->id )
		{
			s += basic_string<char>( "-" );
			s += basic_string<char>( &i->id, 1 );
			s += basic_string<char>( ", ");
		} else {
			s += "    ";
		}

		// If there's a long option, write it
		if( i->name != basic_string<char>( "" ) )
		{
			s += basic_string<char>( "--" );
			s += i->name;
			if( i->hasValue )
			{
				s += basic_string<char>( "=" );
				s += i->valueName;
			}
		}

		cout << s;
		if( s.length() > 28 )
		{
			// It doesn't fit, so output now
			cout << endl;
			for( int i = 0; i < 29; i ++ )
			{
				cout << " ";
			}
		} else {
			for( int i = s.length( ); i < 29; i ++ )
			{
				cout << " ";
			}
		}
		cout << i->doc << endl;
		if( i->values.size( ) )
		{
			for( int j = 0; j < 30; j ++ )
			{
				cout << " ";
			}
			cout << "Options:" << endl;
			for( vector< basic_string<char> >::iterator v = i->values.begin( ); v != i->values.end( ); v ++ )
			{
				for( int j = 0; j < 31; j ++ )
				{
					cout << " ";
				}
				cout << *v << endl;
			}
		}
	}
	cout << endl << endl;
	if( instance.bugAddress != "" ) {
		cout << "Report bugs to " << instance.bugAddress << "." << endl;
	}
	exit( 0 );
}

void Argpc::printUsage( void )
{
	cout << "Usage: " << instance.progName << " ";
	for( vector< ArgpcOption >::iterator i = instance.options.begin( ); i != instance.options.end( ); i ++ )
	{
		if( i->id )
		{
			if( i->hasValue )
			{
				cout << "[-" << i->id << " " << i->valueName << "] ";
			} else {
				cout << "[-" << i->id << "] ";
			}
		}
	}
	for( vector< ArgpcOption >::iterator i = instance.options.begin( ); i != instance.options.end( ); i ++ )
	{
		if( i->name != "" )
		{
			if( i->hasValue )
			{
				cout << "[--" << i->name << "=" << i->valueName << "] ";
			} else {
				cout << "[--" << i->name << "] ";
			}
		}
	}
	cout << instance.argumentDescription << endl;
	exit( 0 );
}

void Argpc::printVersion( void )
{
	cout << instance.progName << " v" << instance.version << endl;
	exit( 0 );
}

UnknownArgumentException::UnknownArgumentException( std::basic_string<char> name ) : name( name )
{
}

UnknownArgumentException::~UnknownArgumentException( ) throw ( )
{
}

const char *UnknownArgumentException::what( void ) const throw( )
{
	return (basic_string<char>( "Unrecognized option: `" ) + name + basic_string<char>( "'" )).c_str( );
}

MissingArgumentException::MissingArgumentException( std::basic_string<char> name ) : name( name )
{
}

MissingArgumentException::~MissingArgumentException( ) throw ( )
{
}

const char *MissingArgumentException::what( void ) const throw( )
{
	return (basic_string<char>( "Argument " ) + name + basic_string<char>( " requires a value" )).c_str( );
}

BadParameterException::BadParameterException( std::basic_string<char> arg, std::basic_string<char> param ) : arg( arg ), param( param )
{
}

BadParameterException::~BadParameterException( ) throw ( )
{
}

const char *BadParameterException::what( void ) const throw( )
{
	return (basic_string<char>("Bad parameter: `") + param + basic_string<char>("' is not a valid option for `") + arg + basic_string<char>("'")).c_str();
}
