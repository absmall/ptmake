#include <assert.h>
#include <sstream>
#include <stdexcept>
#include "make_rules.h"
#include "make_match.h"

using namespace std;

bool MakeRule::match(const string &target, Match **match)
{
    // wildcard offsets in the target and dependencies
    MakeMatch *makeMatch;
    list<string>::iterator b, e, te;

    if( targets == NULL ) return false;

    makeMatch = new MakeMatch;
    b = targets->begin();
    e = targets->end();
    for( te = b; te != e; te ++ ) {
        if( makeMatch->initialize(target, *te) ) {
            *match = makeMatch;
            return true;
        }
    }

    delete makeMatch;
    return false;
}

string MakeRule::expand_command( const string &command, const string &target, Match *m )
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
            case '<':
                // Substitute first dependency
                {
                    string s;
                    s = m->substitute( declaredDeps->begin()->first );
                    ret = ret.replace(position, 2, s);
                    position += s.length();
                }
                break;
            case '^':
                // Substitute all dependencies
                {
                    string s;
                    ret = ret.erase(position, 2);
                    for( list<pair<string,bool> >::iterator i = declaredDeps->begin(); i != declaredDeps->end(); i ++ ) {
                        s = m->substitute( i->first ) + " ";
                        ret = ret.insert(position, s);
                        position += s.length();
                    }
                }
                break;
#if 0
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
