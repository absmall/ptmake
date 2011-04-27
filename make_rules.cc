#include <stdexcept>
#include "re.h"
#include "make_rules.h"

bool MakeRule::match(const std::string &target)
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
