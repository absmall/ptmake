#include <sstream>
#include <stdexcept>
#include "re.h"
#include "make_rules.h"

using namespace std;

bool MakeRule::match(const string &target)
{
	// wildcard offsets in the target and dependencies
	size_t wildcard_t, wildcard_d;
	std::list<std::string>::iterator b, e, te;

	if( targets == NULL ) return false;

	b = targets->begin();
	e = targets->end();
	for( te = b; te != e; te ++ ) {
		if( ( wildcard_t = te->find( '%' ) ) != std::string::npos ) {
			// See if prefix and suffix match
			int length = te->length() - wildcard_t - 1;
			try {
				if( !te->compare( 0, wildcard_t, target, 0, wildcard_t )
				  && !te->compare( wildcard_t + 1, length, target, target.length() - length, length ) ) {
					// We have a match on the pattern. Now see if we can build the listed dependencies
					std::list<std::string>::iterator de;
					if( declaredDeps != NULL ) {
						for(de = declaredDeps->begin(); de != declaredDeps->end(); de ++ ) {
							if( ( wildcard_d = de->find( '%' ) ) != std::string::npos ) {
								stringstream ss;
								string s;
								ss << string( *de, 0, wildcard_d ) << target.substr( wildcard_t, target.length() - length ) << string( *de, wildcard_d+1, string::npos );
								s = ss.str();
								if( !Rule::canBeBuilt( s ) ) {
									return false;
								}
							}
						}
					}
					return true;
				}
			} catch(std::out_of_range &o) {
				// Not a match, but keep checking
			}
		} else {
			if( ::match( *te, target ) ) {
				return true;
			}
		}
	}
	return false;
}

string MakeRule::expand_command( const string &command, const string &target )
{
	string ret = command;
	size_t position = 0;

	while( true ) {
		position = ret.find('$', position);
		if( position == std::string::npos ) break;

		// If it's the last character, do nothing special
		if( position == ret.length() - 1 ) return ret;

		// See what follows
		switch( ret[ position + 1 ] ) {
			case '@':
				// Substitute targets
				ret = ret.replace(position, 2, target);
				position += target.length();
				break;
#if 0
			case '<':
				// Substitute first dependency
			case '^':
				// Substitute all dependencies
			case '(':
				// Substitute variable
#endif
			case '$':
				// Substitute $
				ret = ret.erase(position+1, position+2);
				position++;
				break;
			default:
				// Don't expand
				position ++;
		}
	}
	return ret;
}
