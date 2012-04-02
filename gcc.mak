# Filter out the options we can parse
CPPFLAGS:=$(filter warnings debug, $(BUILD_OPTIONS))
CXXFLAGS:=$(filter warnings debug, $(BUILD_OPTIONS))
LDFLAGS:=$(filter debug, $(BUILD_OPTIONS))

CPPFLAGS:=$(subst warnings,-Wall,$(CPPFLAGS))
CPPFLAGS:=$(subst debug,-DDEBUG -g3,$(CPPFLAGS))
CXXFLAGS:=$(subst warnings,-Wall,$(CXXFLAGS))
CXXFLAGS:=$(subst debug,-DDEBUG -g3,$(CXXFLAGS))
LDFLAGS:=$(subst debug,-g3,$(LDFLAGS))

CC=gcc -c $(CPPFLAGS)
CXX=g++ -c $(CXXFLAGS)
LD=g++ $(LDFLAGS) $(LIBS)
OUTPUT=-o

%.o : %.cc
	g++ -c $(CXXFLAGS) `libgcrypt-config --cflags` -o $@ $<

%.o : %.c
	gcc -c $(CPPFLAGS) `libgcrypt-config --cflags` -o $@ $<

libptmake.so : CXXFLAGS += -fPIC
libptmake.so : $(OBJS)
	$(LD) -shared -Wl,-soname,libptmake.so `libgcrypt-config --cflags --libs` -ldb -o $@ $^

# Look for the library in the current directory for a debug build, so we don't
# need to install it all the time
ifeq ($(filter debug, $(BUILD_OPTIONS)),debug)
LSEARCH=-Wl,-rpath,. -L.
endif

ifeq ($(filter make, $(BUILD_OPTIONS)),make)
all : make
make : make.o make_parse.o make_rules.o make_match.o main.o find.o libptmake.so
	$(LD) $(LSEARCH) -o $@ make.o make_rules.o make_match.o make_parse.o main.o find.o -lptmake
endif

ifeq ($(filter jam, $(BUILD_OPTIONS)),jam)
all : jam
jam : jam.o jam_parse.o main.o find.o libptmake.so
	$(LD) $(LSEARCH) -o $@ jam.o jam_parse.o main.o find.o -lptmake
endif
