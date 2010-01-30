#ifndef __ARGPC__
#define __ARGPC__

#include <string>
#include <vector>
#include <argpcoption.h>
#include <exception.h>

class Argpc
{
public:
	void addOption( const ArgpcOption &o );
	void addOption( std::string s, const ArgpcOption &o );

	void addHelp( void );
	void addUsage( void );
	void addVersion( void );

	void parsePartial( int *argc, char *argv[] );
	void setVersion( std::string s );
	void parse( int *argc, char *argv[] );
	void setDoc( std::string s );
	void setBugAddress( std::string s );
	void setArgumentDescription( std::string s );
	void displayArgumentError( std::string s );
	static Argpc *getInstance( void );

	static void printHelp( void );
	static void printUsage( void );
	static void printVersion( void );
private:
	Argpc( void );
	~Argpc( void );

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
