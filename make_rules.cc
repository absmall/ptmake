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
		if( ( wildcard = te->find( '%' ) ) ) {
			// See if prefix and suffix match
			int length = te->length() - wildcard - 1;
			if( !te->compare( 0, wildcard, target, 0, wildcard )
			  && !te->compare( wildcard + 1, length, target, target.length() - length, length ) ) {
				return true;
			}
		} else {
			if( ::match( *te, target ) ) {
				return true;
			}
		}
	}
	return false;
}
