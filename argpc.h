#ifndef __ARGPC__
#define __ARGPC__

#include <string>
#include <vector>
#include "argpcoption.h"
#include "exception.h"

/*
 * Class for parsing command line arguments.
 * 
 * Usage:
 * 1) Create an Argpc object
 * 2) Call addOption once for each possible argument to parse.
 * 3) Call addHelp, addVersion and addUsage to add a few options standard to
 *    most gnu programs.
 * 4) Call parsePartial if it is necessary to parse some arguments. This makes
 *    it possible to respond to some arguments in a different way based on
 *    earlier arguments.
 * 5) Call parse which will parse all arguments
 */
class Argpc
{
public:
	/* Specify a command line option that should be parsed if a user specifies
	 * it */
	void addOption( const ArgpcOption &o );
	/* Specify a command line option that should be parsed if a user specifies
	 * it. Group allows grouping some parameters under a header in --help */
	void addOption( std::string group, const ArgpcOption &o );

	/* Generate a help option in a standard format like gnu programs */
	void addHelp( void );
	/* Generate a usage option in a standard format like gnu programs */
	void addUsage( void );
	/* Generate a version option in a standard format like gnu programs */
	void addVersion( void );

	/* Set the program version, printed out in the default help */
	void setVersion( std::string s );

	/* Set the documentation string, printed out in help and usage */
	void setDoc( std::string s );

	/* Set the address to report bugs, printed out in help and usage */
	void setBugAddress( std::string s );

	/* Set the description of non-flag arguments. Printed out in the usage */
	void setArgumentDescription( std::string s );

	/* Parse out arguments that have been specified, ignore unknown arguments.
	 * They can be parsed with a subsequent parsePartial or parse call.
	 */
	void parsePartial( int *argc, char *argv[] );

	/* Parse all arguments. Give an error when unexpected arguments
	 * are encountered.
	 */
	void parse( int *argc, char *argv[] );

	/*
	 * Get the reference, because this class is generally singleton
	 */
	static Argpc *getInstance( void );

private:
	Argpc( void );
	~Argpc( void );

	static void printHelp( void );
	static void printUsage( void );
	static void printVersion( void );
	void displayArgumentError( std::string s );
	void removeArg( int *argc, char *argv[], int i );
	void parseShortOption( int *argc, char *argv[], int i );
	void parseLongOption( int *argc, char *argv[], int i );
	static bool initialized;
	static Argpc instance;
	bool hasHelp;
	bool hasUsage;
	std::string progName;
	std::string doc;
	std::string version;
	std::string bugAddress;
	std::string argumentDescription;
	std::vector<ArgpcOption> options;
};

class MissingArgumentException : public wexception
{
public:
	MissingArgumentException( std::string name );
	virtual ~MissingArgumentException( void ) throw( );
	virtual const char *what( void ) const throw( );
private:
	std::string name;
};

class UnknownArgumentException : public wexception
{
public:
	UnknownArgumentException( std::string name );
	virtual ~UnknownArgumentException( void ) throw( );
	virtual const char *what( void ) const throw( );
private:
	std::string name;
};


#endif /* __ARGPC__ */
