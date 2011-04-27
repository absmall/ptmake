OS := $(shell uname -o)
ifeq ($(OS),GNU/Linux)
ENVIRONMENT=gcc
else
ENVIRONMENT=vc
endif

BUILD_OPTIONS=warnings debug make

OBJS = main.o build.o find.o argpc.o argpcoption.o exception.o rules.o dependencies.o graphviz.o utilities.o debug.o re.o variables.o

ifeq ($(filter make, $(BUILD_OPTIONS)),make)
OBJS += make.o make_rules.o
endif

ifeq ($(filter jam, $(BUILD_OPTIONS)),jam)
OBJS += jam.o
endif

ifeq ($(ENVIRONMENT),vc)
OBJS += subprocess_win.o
endif

ifeq ($(ENVIRONMENT),gcc)
OBJS += subprocess_unix.o
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

all: make

clean:
	rm -f *.o make.cc make.hh make.h jam.cc jam.hh jam.h make

%.hh %.cc:%.y
	$(YACC) -d -o $*.cc $<
