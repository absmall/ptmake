make:
	g++ -g3 -Wl,-rpath,. -L. -o make make.o make_parse.o make_rules.o main.o find.o -lptmake

jam:
	g++ -g3 -Wl,-rpath,. -L. -o jam jam.o jam_parse.o main.o find.o -lptmake

libptmake.so:
	g++ -g3 -shared -Wl,-soname,libptmake.so `libgcrypt-config --cflags --libs` -ldb -o libptmake.so build.o argpc.o argpcoption.o exception.o rules.o dependencies.o graphviz.o utilities.o debug.o subprocess_unix.o file_unix.o re.o variables.o

%.o: %.cc
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -fPIC -g3 `libgcrypt-config --cflags` -o $@ $<

%.cc: %.y
	bison -d -o $@ jam_parse.y
