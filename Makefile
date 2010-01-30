OBJS = main.o find.o parse.o argpc.o argpcoption.o exception.o parser.o

CPPFLAGS = -g3 -I.

.SUFFIXES: 

all: make

clean:
	rm -f *.o

%.o:%.cc
	g++ -c $(CPPFLAGS) -o $@ $<

%.o:%.c
	gcc -c $(CPPFLAGS) -o $@ $<

%.c %.h:%.y
	yacc -d -o $@ $<

make: $(OBJS)
	g++ -g3 $(LDFLAGS) -o $@ $^

parse.o: | parser.h
