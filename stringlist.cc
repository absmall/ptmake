#include <string>
#include <list>

using namespace std;

void *new_stringlist()
{
	return new list<string>;
}

void *add_string(void *stringlist, void *string)
{
	list<string> *_stringlist = (list<string> *)stringlist;
	string *_stringlist = (string *)stringlist;

	_stringlist->push_back( _string );

	return _stringlist;
}
