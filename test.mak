make.exe:
	g++ -DYYDEBUG -g3 `libgcrypt-config --cflags --libs` -o make.exe main.o build.o find.o file_unix.o argpc.o argpcoption.o subprocess_unix.o exception.o rules.o parser.o

main.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o main.o main.cc

build.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o build.o build.cc

find.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o find.o find.cc

argpc.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o argpc.o argpc.cc

argpcoption.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o argpcoption.o argpcoption.cc

exception.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o exception.o exception.cc

rules.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o rules.o rules.cc

parser.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o parser.o parser.cc

file_unix.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o file_unix.o file_unix.cc

subprocess_unix.o:
	g++ -c -Wall -DYYDEBUG -DDEBUG -g3 `libgcrypt-config --cflags` -o subprocess_unix.o subprocess_unix.cc

parser.cc:
	bison -d -o parser.cc parser.y

