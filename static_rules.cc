#include "rules.h"

void static_rules()
{
		Rule *r = new Rule();
		r->addTarget("main.o");
		r->addCommand("g++ -c -Wall -DDEBUG -g3 -o main.o main.cc");
}
