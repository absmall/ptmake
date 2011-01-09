OS := $(shell uname -o)
ifeq ($(OS),GNU/Linux)
ENVIRONMENT=gcc
else
ENVIRONMENT=vc
endif

OBJS = main.o build.o find.o argpc.o argpcoption.o exception.o rules.o parser.o dependencies.o

ifeq ($(ENVIRONMENT),vc)
OBJS += subprocess_win.o
endif

ifeq ($(ENVIRONMENT),gcc)
OBJS += subprocess_unix.o
OBJS += file_unix.o
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

all: make

clean:
	rm -f *.o parser.cc parser.hh make.exe

%.hh %.cc:%.y
	$(YACC) -d -o $*.cc $<
