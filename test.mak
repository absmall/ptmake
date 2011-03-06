make:
	g++ -g3 -Wl,-rpath,. -L. -o make make.o make_parse.o main.o find.o -lptmake

jam:
	g++ -g3 -Wl,-rpath,. -L. -o jam jam.o jam_parse.o main.o find.o -lptmake

libptmake.so:
	g++ -g3 -shared -Wl,-soname,libptmake.so `libgcrypt-config --cflags --libs` -ldb -o libptmake.so build.o argpc.o argpcoption.o exception.o rules.o dependencies.o graphviz.o utilities.o debug.o subprocess_unix.o file_unix.o

main.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o main.o main.cc

build.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o build.o build.cc

find.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o find.o find.cc

argpc.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o argpc.o argpc.cc

argpcoption.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o argpcoption.o argpcoption.cc

graphviz.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o graphviz.o graphviz.cc

exception.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o exception.o exception.cc

rules.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o rules.o rules.cc

make.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o make.o make.cc

file_unix.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o file_unix.o file_unix.cc

dependencies.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o dependencies.o dependencies.cc

subprocess_unix.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o subprocess_unix.o subprocess_unix.cc

utilities.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o utilities.o utilities.cc

debug.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 -fPIC `libgcrypt-config --cflags` -o debug.o debug.cc

make_parse.cc:
	bison -o make_parse.cc make_parse.y

jam_parse.cc:
	bison -o jam_parse.cc jam_parse.y
