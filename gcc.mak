# Filter out the options we can parse
CPPFLAGS:=$(filter warnings debug, $(BUILD_OPTIONS))
CXXFLAGS:=$(filter warnings debug, $(BUILD_OPTIONS))
LDFLAGS:=$(filter debug, $(BUILD_OPTIONS))

CPPFLAGS:=$(subst warnings,-Wall,$(CPPFLAGS))
CPPFLAGS:=$(subst debug,-DYYDEBUG -DDEBUG -g3,$(CPPFLAGS))
CXXFLAGS:=$(subst warnings,-Wall,$(CXXFLAGS))
CXXFLAGS:=$(subst debug,-DYYDEBUG -DDEBUG -g3,$(CXXFLAGS))
LDFLAGS:=$(subst debug,-DYYDEBUG -g3,$(LDFLAGS))

CC=gcc -c $(CPPFLAGS)
CXX=g++ -c $(CXXFLAGS)
LD=g++ $(LDFLAGS) $(LIBS)
OUTPUT=-o

%.o : %.cc
	g++ -c $(CXXFLAGS) `libgcrypt-config --cflags` -o $@ $<

%.o : %.c
	gcc -c $(CPPFLAGS) `libgcrypt-config --cflags` -o $@ $<

make : $(OBJS)
	$(LD) `libgcrypt-config --cflags --libs` -ldb -o $@ $^
