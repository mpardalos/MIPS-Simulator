CXX=clang++

ARTIFACTNAME=out
DIST=dist
CPPFLAGS=-I include/ -Wall

src=$(wildcard src/*.cpp)
headers=$(wildcard src/*.hpp)
objects=$(src:.cpp=.o)

build: $(objects)
	mkdir -p $(DIST)
	$(CXX) $(LINKOPTS) -o $(DIST)/$(ARTIFACTNAME) $^

check:
	$(CXX) $(CPPFLAGS) -fsyntax-only $(src) $(headers)

.PHONY: clean run
run:
	@$(DIST)/$(ARTIFACTNAME)

clean:
	rm -rf $(objects) $(DIST)
