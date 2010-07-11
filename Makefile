OS := $(shell uname -o)
ifeq ($(OS),GNU/Linux)
ENVIRONMENT=gcc
else
ENVIRONMENT=vc
endif

OBJS = main.o build.o find.o argpc.o argpcoption.o exception.o rules.o

ifeq ($(ENVIRONMENT),vc)
OBJS += subprocess_win.o
endif

ifeq ($(ENVIRONMENT),gcc)
OBJS += subprocess_unix.o
endif

BUILD_OPTIONS=warnings debug

include bison.mak

ifeq ($(ENVIRONMENT),vc)
include vc.mak
endif
ifeq ($(ENVIRONMENT),gcc)
include gcc.mak
endif

.SUFFIXES: 
.PHONY:clean

all: make.exe

clean:
	rm -f *.o parser.c parser.h make.exe

%.h %.c:%.y
	$(YACC) -d -o $*.c $<

parse.o: | parser.h
