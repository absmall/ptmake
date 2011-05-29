OS := $(shell uname -s)
ifeq ($(findstring Linux, $(OS)),Linux)
ENVIRONMENT=gcc
else
ifeq ($(findstring Darwing, $(OS)),Darwin)
ENVIRONMENT=gcc
else
ENVIRONMENT=vc
endif
endif

all: 
BUILD_OPTIONS=warnings debug make jam

OBJS = build.o argpc.o argpcoption.o exception.o rules.o dependencies.o graphviz.o utilities.o debug.o

ifeq ($(ENVIRONMENT),vc)
OBJS += subprocess_win.o
endif

ifeq ($(findstring Linux, $(OS)),Linux)
OBJS += subprocess_unix.o
OBJS += file_unix.o
endif

ifeq ($(findstring Darwin, $(OS)),Darwin)
OBJS += subprocess_mach.o
OBJS += file_unix.o
endif

include bison.mak

ifeq ($(ENVIRONMENT),vc)
include vc.mak
endif
ifeq ($(ENVIRONMENT),gcc)
include gcc.mak
endif

.SUFFIXES: 
.PHONY:clean

clean:
	rm -f *.o make_parse.cc make_parse.hh jam_parse.cc jam_parse.hh make jam libptmake.so

%.hh %.cc:%.y
	$(YACC) -o $*.cc $<
