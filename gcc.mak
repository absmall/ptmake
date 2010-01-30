# Filter out the options we can parse
CPPFLAGS:=$(filter warnings debug, $(BUILD_OPTIONS))
CXXFLAGS:=$(filter warnings debug, $(BUILD_OPTIONS))
LDFLAGS:=$(filter debug, $(BUILD_OPTIONS))

CPPFLAGS:=$(subst warnings,-Wall,$(CPPFLAGS))
CPPFLAGS:=$(subst debug,-g3,$(CPPFLAGS))
CXXFLAGS:=$(subst warnings,-Wall,$(CXXFLAGS))
CXXFLAGS:=$(subst debug,-g3,$(CXXFLAGS))
LDFLAGS:=$(subst debug,-g3,$(LDFLAGS))

CC=gcc -c $(CPPFLAGS)
CXX=g++ -c $(CXXFLAGS)
LD=g++ $(LDFLAGS)
OUTPUT=-o

%.o : %.cc
	g++ -c $(CXXFLAGS) -o $@ $<

%.o : %.c
	gcc -c $(CPPFLAGS) -o $@ $<

make.exe : $(OBJS)
	g++ $(LDFLAGS) -o $@ $^
