CXX=clang++

ARTIFACTNAME=out
DIST=dist
CPPFLAGS=-I include/ -Wall -std=c++11

src=$(wildcard src/*.cpp)
headers=$(wildcard src/*.hpp)
objects=$(src:.cpp=.o)

all: build asm

build: $(objects)
	mkdir -p $(DIST)
	$(CXX) $(LINKOPTS) -o $(DIST)/$(ARTIFACTNAME) $^

check:
	$(CXX) $(CPPFLAGS) -fsyntax-only $(src) $(headers)

asm:
	$(MAKE) -C ollys_asm
	cp ollys_asm/bin/parser ./testbench/asm

.PHONY: clean run
run:
	@$(DIST)/$(ARTIFACTNAME)

clean:
	rm -rf $(objects) $(DIST)

	$(MAKE) -C ollys_asm clean
	rm -f testbench/asm
