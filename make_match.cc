#include <stdexcept>
#include <sstream>
#include "make_match.h"

using namespace std;

bool MakeMatch::initialize(const string &target, const string &pattern)
{
    this->target = target;
    this->pattern = pattern;

    if( ( wildcard_t = pattern.find( '%' ) ) != std::string::npos ) {
        // See if prefix and suffix match
        int length = pattern.length() - wildcard_t - 1;
        try {
            if( !pattern.compare( 0, wildcard_t, target, 0, wildcard_t )
                    && !pattern.compare( wildcard_t + 1, length, target, target.length() - length, length ) ) {
                wildcard = true;
                return true;
            } else {
                // No match
                return false;
            }
        } catch(std::out_of_range &o) {
            return false;
        }
    } else {
        wildcard = false;
        return target == pattern;
    }
}

string MakeMatch::substitute(const string &input)
{
    string ret;

    // wildcard offsets in the target and dependencies
    size_t wildcard_i;

    if( wildcard ) {
        int length = pattern.length() - wildcard_t - 1;
        if( (wildcard_i = input.find( '%' )) == std::string::npos ) {
            return input;
        } else {
            stringstream ss;
            string s;
            ss << string( input, 0, wildcard_i ) << target.substr( wildcard_t, target.length() - length ) << string( input, wildcard_i+1, string::npos );
            return ss.str();
        }
    } else {
        return input;
    }
}
