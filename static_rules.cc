#include "rules.h"

void static_rules()
{
#if 0
	Rule *make = new Rule();
	make->addTarget("make.exe");
	make->addCommand("./a.out b.out");

	Rule *make2 = new Rule();
	make2->addTarget("b.out");
	make2->addCommand("./b.out");
#endif

	Rule *make = new Rule();
	make->addTarget("make.exe");
	make->addCommand("g++ -DYYDEBUG -g3 -o make.exe main.o build.o find.o argpc.o argpcoption.o exception.o rules.o static_rules.o file_unix.o subprocess_unix.o");

	Rule *main = new Rule();
	main->addTarget("main.o");
	main->addCommand("g++ -c -Wall -DDEBUG -g3 -o main.o main.cc");

	Rule *build = new Rule();
	build->addTarget("build.o");
	build->addCommand("g++ -c -Wall -DDEBUG -g3 -o build.o build.cc");

	Rule *find = new Rule();
	find->addTarget("find.o");
	find->addCommand("g++ -c -Wall -DDEBUG -g3 -o find.o find.cc");

	Rule *argpc = new Rule();
	argpc->addTarget("argpc.o");
	argpc->addCommand("g++ -c -Wall -DDEBUG -g3 -o argpc.o argpc.cc");

	Rule *argpcoption = new Rule();
	argpcoption->addTarget("argpcoption.o");
	argpcoption->addCommand("g++ -c -Wall -DDEBUG -g3 -o argpcoption.o argpcoption.cc");

	Rule *exception = new Rule();
	exception->addTarget("exception.o");
	exception->addCommand("g++ -c -Wall -DDEBUG -g3 -o exception.o exception.cc");

	Rule *rules = new Rule();
	rules->addTarget("rules.o");
	rules->addCommand("g++ -c -Wall -DDEBUG -g3 -o rules.o rules.cc");

	Rule *static_rules = new Rule();
	static_rules->addTarget("static_rules.o");
	static_rules->addCommand("g++ -c -Wall -DDEBUG -g3 -o static_rules.o static_rules.cc");

	Rule *file_rules = new Rule();
	file_rules->addTarget("file_unix.o");
	file_rules->addCommand("g++ -c -Wall -DDEBUG -g3 -o file_unix.o file_unix.cc");

	Rule *subprocess_unix = new Rule();
	subprocess_unix->addTarget("subprocess_unix.o");
	subprocess_unix->addCommand("g++ -c -Wall -DDEBUG -g3 -o subprocess_unix.o subprocess_unix.cc");
}
