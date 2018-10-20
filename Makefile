CXX=clang++

ARTIFACTNAME=out
DIST=dist
CPPFLAGS=-I include/ -Wall

objects=src/main.o src/decoder.o src/runtime.o

build: $(objects)
	mkdir -p $(DIST)
	$(CXX) $(LINKOPTS) -o $(DIST)/$(ARTIFACTNAME) $^

.PHONY: clean run
run:
	@$(DIST)/$(ARTIFACTNAME)

clean:
	rm -rf $(objects) $(DIST)