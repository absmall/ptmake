all: make libptmake.so

make: make.o make_rules.o make_parse.o make_match.o main.o find.o 
	g++ -g3 -Wl,-rpath,. -L. -o $@ $^ -lptmake

jam: jam.o jam_parse.o main.o find.o 
	g++ -g3 -Wl,-rpath,. -L. -o $@ $^ -lptmake

libptmake.so: build.o argpc.o argpcoption.o exception.o rules.o dependencies.o plotter.o utilities.o debug.o subprocess_unix.o file_unix.o variables.o match.o
	g++ -g3 -shared -Wl,-soname,$@ `libgcrypt-config --cflags --libs` -ldb -o $@ $^

%.o: %.cc
	g++ -c -Wall -DDEBUG -fPIC -g3 `libgcrypt-config --cflags` -o $@ $<

%.cc: %.y
	bison -d -o $@ $<
