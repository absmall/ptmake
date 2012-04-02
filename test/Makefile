all: test test_interactive

CXXFLAGS = -g3

%.o: %.cc
	g++ $(CXXFLAGS) -I.. -c -o $@ $<

test: main.cc deps.o make_rules.o ../make_rules.o ../make_match.o
	g++ $(CXXFLAGS) -Wl,-rpath,.. -L.. -o $@ $^ -lptmake -lcunit

test_interactive: CXXFLAGS += -DINTERACTIVE
test_interactive: main.cc deps.o make_rules.o ../make_rules.o ../make_match.o
	g++ $(CXXFLAGS) -Wl,-rpath,.. -L.. -o $@ $^ -lptmake -lcunit	
