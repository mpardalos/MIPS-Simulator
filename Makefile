CXX=g++
SIMULATOR_BIN_NAME=mips_simulator
DIST=bin

.PHONY: clean run testbench
.DEFAULT_GOAL := all

# --------------- Simulator --------------- 

# DEBUG_FLAGS=-DDEBUG -DBREAK_IS_REGDUMP
CPPFLAGS=-I include/ -Wall -Wextra -Wno-c++14-binary-literal -std=c++11 $(DEBUG_FLAGS)
src=$(wildcard src/*.cpp)
headers=$(wildcard src/*.hpp)
objects=$(src:.cpp=.o)

simulator: $(objects)
	mkdir -p $(DIST)
	$(CXX) $(LINKOPTS) -o $(DIST)/$(SIMULATOR_BIN_NAME) $^

check:
	$(CXX) $(CPPFLAGS) -fsyntax-only $(src) $(headers)

# --------------- Testbench --------------- 
LINK_SCRIPT=testbench/linker.ld
MIPS_AS = mips-linux-gnu-as
MIPS_LD = mips-linux-gnu-ld
MIPS_OBJCOPY = mips-linux-gnu-objcopy
MIPS_OBJDUMP = mips-linux-gnu-objdump
MIPS_ASFLAGS = -march=mips1 -mfp32 -mabi=32
MIPS_LDFLAGS = -nostdlib -melf32btsmip --gpsize=0 -static -Bstatic --build-id=none --entry=0000000010000000

testsrc=$(wildcard testbench/tests/*.s)
testobjects=$(testsrc:.s=.mips.o)
testelf=$(testsrc:.s=.mips.elf)
testbins=$(testsrc:.s=.mips.bin)
.PRECIOUS: $(testelf)

testbench_script=testbench/mips_testbench

tests: $(testbins)

testbench: tests $(testbench_script)
	mkdir -p $(DIST)/tests
	@ echo "Copying testbench"
	@ cp -r testbench/mips_testbench $(DIST)
	@ echo "Copying test binaries"
	@ cp -r $(testbins) $(DIST)/tests
	@ echo "Copying test info files"
	@ cp -r testbench/tests/*.info $(DIST)/tests

test: testbench simulator $(testbench)
	$(DIST)/mips_testbench $(DIST)/$(SIMULATOR_BIN_NAME) 2>/dev/null

pretty_test: testbench simulator $(testbench)
	$(DIST)/mips_testbench $(DIST)/$(SIMULATOR_BIN_NAME) 2>/dev/null | column -t -s',|' | scripts/highlight red "Fail" | scripts/highlight green "Pass"

# Assemble MIPS assembly file (.s) into MIPS object file (.o)
%.mips.o: %.s
	@ echo "Assembling $@"
	@ $(MIPS_AS) $(MIPS_ASFLAGS) $< -o $@

# Link MIPS object file (.o), producing .elf, using memory locations specified in spec
%.mips.elf: %.mips.o
	@ echo "Linking $@"
	@ $(MIPS_LD) $(MIPS_LDFLAGS) -T $(LINK_SCRIPT) $< -o $@

# Extract binary instructions only from linked object file (.elf)
%.mips.bin: %.mips.elf
	@ echo "Extracting instructions to $@"
	@ $(MIPS_OBJCOPY) -O binary --only-section=.text $< $@

# --------------- Helpers --------------- 

run: simulator
	@$(DIST)/$(ARTIFACTNAME)

all: simulator testbench

clean:
	rm -rf $(objects) $(testobjects) $(testelf) $(testbins) $(DIST)

rebuild: 
	$(MAKE) clean
	$(MAKE)
