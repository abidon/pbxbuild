CC=clang
CXX=clang++
LD=clang++

CXXFLAGS=-std=c++11 -stdlib=libc++

CFILES=$(wildcard pbxbuild/*.c)
COBJS=$(patsubst %.c, %.o, ${CFILES})
CXXFILES=$(wildcard pbxbuild/*.cc) libpbxparser/plist_decoder.cc libpbxparser/value.cc
CXXOBJS=$(patsubst %.cc, %.o, ${CXXFILES})

all: pbxbuild/pbxbuild

pbxbuild/pbxbuild: $(COBJS) $(CXXOBJS)
	clang++ -std=c++11 -stdlib=libc++ -lc++abi -o $@ $^

clean:
	$(RM) pbxbuild/pbxbuild $(COBJS) $(CXXOBJS)
