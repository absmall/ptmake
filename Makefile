OBJS = main.o build.o find.o parse.o argpc.o argpcoption.o exception.o parser.o rules.o

CPPFLAGS = -Wall -g3

.SUFFIXES: 

all: make.exe

clean:
	rm -f *.o

%.o:%.cc
	g++ -c $(CPPFLAGS) -o $@ $<

%.o:%.c
	gcc -c $(CPPFLAGS) -o $@ $<

%.h %.c:%.y
	yacc -d -o $*.c $<

make.exe: $(OBJS)
	g++ -g3 $(LDFLAGS) -o $@ $^

parse.o: | parser.h
