#include <iostream>
#include "file.h"

using namespace std;

int main()
{
    cout << fileCanonicalize( "hello" ) << endl;
    cout << endl;
    cout << fileCanonicalize( "/hello" ) << endl;
    cout << endl;
    cout << fileCanonicalize( "./hello" ) << endl;
    cout << endl;
    cout << fileCanonicalize( "/./hello" ) << endl;
    cout << endl;
    cout << fileCanonicalize( "./../hello" ) << endl;
    cout << endl;
    cout << fileCanonicalize( "/bye/../hello" ) << endl;
    cout << endl;
    return 0;
}
