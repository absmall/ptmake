OBJS = main.o build.o find.o parse.o argpc.o argpcoption.o exception.o parser.o rules.o

BUILD_OPTIONS=warnings debug

include bison.mak
#include gcc.mak
include vc.mak

.SUFFIXES: 
.PHONY:clean

all: make.exe

clean:
	rm -f *.o parser.c parser.h make.exe

%.h %.c:%.y
	$(YACC) -d -o $*.c $<

parse.o: | parser.h
