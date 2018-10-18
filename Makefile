CXX=clang++

ARTIFACTNAME=out
DIST=dist
CPPFLAGS=

objects=src/main.o

build: $(objects)
	mkdir -p $(DIST)
	$(CXX) $(LINKOPTS) -o $(DIST)/$(ARTIFACTNAME) $^

.PHONY: clean run
run:
	@$(DIST)/$(ARTIFACTNAME)

clean:
	rm -rf $(objects) $(DIST)