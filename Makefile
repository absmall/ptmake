OBJS = main.o find.o aprpp.o parse.o argpc.o argpcoption.o exception.o parser.o

CPPFLAGS = -g3 -I. `pkg-config --cflags apr-1`
LDFLAGS = `pkg-config --libs apr-1`

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

parse.o: parser.h
