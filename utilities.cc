#include "utilities.h"

using namespace std;

string printhash(unsigned char hash[32])
{
	char buf[64];
	int i;
	char c;

	for(i=0; i < 32; i ++ ) {
		c = hash[i] >> 4;
		if( c < 10 ) {
			buf[i*2] = c+'0';
		} else {
			buf[i*2] = c-10+'A';
		}
		c = hash[i] & 31;
		if( c < 10 ) {
			buf[i*2+1] = c+'0';
		} else {
			buf[i*2+1] = c-10+'A';
		}
	}

	return string(buf, 64);
}
