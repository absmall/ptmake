#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "file.h"
#include "exception.h"

bool fileTime(const std::string &file, time_t &time)
{
	struct stat s;

	if( stat( file.c_str(), &s ) ) {
		return false;
	}

	time = s.st_mtime;
	return true;
}

time_t fileTime(const std::string &file)
{
	struct stat s;

	if( stat( file.c_str(), &s ) ) {
		throw runtime_wexception( "Cannot access file time" );
	}

	return s.st_mtime;
}

time_t fileTimeEarliest(const std::list<std::string> &files)
{
	std::list<std::string>::const_iterator i;
	time_t earliest, t;

	if( files.empty() ) {
		throw runtime_wexception( "Retrieving timestamp with no files" );
	}

	i = files.begin();

	earliest = fileTime( *i );

	while( i != files.end() ) {
		t = fileTime( *i );
		if( t < earliest ) earliest = t;
		i ++;
	}

	return earliest;
}
