#include "hash.h"

std::ostream &operator<<(std::ostream &stream, const Hash &hash)
{
    char buf[65];
    int i;
    char c;

    for(i=0; i < 32; i ++ ) {
        c = hash.data()[i] >> 4;
        if( c < 10 ) {
            buf[i*2] = c+'0';
        } else {
            buf[i*2] = c-10+'A';
        }
        c = hash.data()[i] & 31;
        if( c < 10 ) {
            buf[i*2+1] = c+'0';
        } else {
            buf[i*2+1] = c-10+'A';
        }
    }
    
    buf[64] = 0;

    stream << buf;

    return stream;
}
