OBJS = main.o find.o parse.o argpc.o argpcoption.o exception.o parser.o rules.o

CPPFLAGS = -Wall -g3 -I.

.SUFFIXES: 

all: make.exe

clean:
	rm -f *.o

%.o:%.cc
	g++ -c $(CPPFLAGS) -o $@ $<

%.o:%.c
	gcc -c $(CPPFLAGS) -o $@ $<

%.c %.h:%.y
	yacc -d -o $@ $<

make.exe: $(OBJS)
	g++ -g3 $(LDFLAGS) -o $@ $^

parse.o: | parser.h
