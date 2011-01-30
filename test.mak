make:
	g++ -ldb -DMAKEFILE -DYYDEBUG -g3 `libgcrypt-config --cflags --libs` -o make main.o build.o find.o file_unix.o argpc.o argpcoption.o subprocess_unix.o exception.o rules.o make.o dependencies.o graphviz.o utilities.o debug.o re.o

main.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o main.o main.cc

build.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o build.o build.cc

find.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o find.o find.cc

argpc.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o argpc.o argpc.cc

argpcoption.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o argpcoption.o argpcoption.cc

graphviz.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o graphviz.o graphviz.cc

exception.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o exception.o exception.cc

rules.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o rules.o rules.cc

make.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o make.o make.cc

file_unix.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o file_unix.o file_unix.cc

dependencies.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o dependencies.o dependencies.cc

subprocess_unix.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o subprocess_unix.o subprocess_unix.cc

utilities.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o utilities.o utilities.cc

debug.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o debug.o debug.cc

re.o:
	g++ -c -Wall -DMAKEFILE -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o re.o re.cc

make.cc:
	bison -d -o make.cc make.y

jam.cc:
	bison -d -o jam.cc jam.y
