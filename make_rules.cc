#include <stdexcept>
#include "re.h"
#include "make_rules.h"

using namespace std;

bool MakeRule::match(const string &target)
{
	size_t wildcard;
	std::list<std::string>::iterator b, e, te;

	if( targets == NULL ) return false;

	b = targets->begin();
	e = targets->end();
	for( te = b; te != e; te ++ ) {
		if( ( wildcard = te->find( '%' ) ) != std::string::npos ) {
			// See if prefix and suffix match
			int length = te->length() - wildcard - 1;
			try {
				if( !te->compare( 0, wildcard, target, 0, wildcard )
				  && !te->compare( wildcard + 1, length, target, target.length() - length, length ) ) {
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
