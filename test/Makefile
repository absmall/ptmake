%.o: %.cc
	g++ -g3 -I.. -c -o $@ $<

test: main.o deps.o make_rules.o ../make_rules.o
	g++ -g3 -Wl,-rpath,.. -L.. -o test $^ -lptmake -lcunit
