CXX=clang++

ARTIFACTNAME=out
DIST=dist
CPPFLAGS=-I src/include/

objects=src/main.o src/decoder.o

build: $(objects)
	mkdir -p $(DIST)
	$(CXX) $(LINKOPTS) -o $(DIST)/$(ARTIFACTNAME) $^

.PHONY: clean run
run:
	@$(DIST)/$(ARTIFACTNAME)

clean:
	rm -rf $(objects) $(DIST)