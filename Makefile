OBJS = main.o find.o aprpp.o parse.o argpc.o argpcoption.o exception.o

CPPFLAGS = -g3 -I. `pkg-config --cflags apr-1`
LDFLAGS = `pkg-config --libs apr-1`

.SUFFIXES: 

all: make

clean:
	rm -f *.o

%.o:%.cc
	g++ -c $(CPPFLAGS) -o $@ $^

make: $(OBJS)
	g++ -g3 $(LDFLAGS) -o $@ $^
