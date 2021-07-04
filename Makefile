# Build details

CXX = g++
CXXFLAGS = -W -Wall # -g
BD = ./build

# Compile to objects

$(BD)/%.o: %.cpp
	-$(if $(wildcard $(BD)),,mkdir -p $(BD))
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Build Executable

bin = csize

all: $(bin)

rebuild: clean all

# Executable 1

obj = main.o lzw.o huffman.o
objbd = $(obj:%=$(BD)/%)

$(bin): $(objbd)
	$(CXX) -o $(bin) $(objbd) $(CXXFLAGS)

# Dependencies

$(BD)/lzw.o: lzw.h
$(BD)/huffman.o: huffman.h
$(BD)/main.o: huffman.h lzw.h

# Clean up

clean:
	rm -f "$(bin)" $(objbd)
	rm -fd "$(BD)"

# PHONY

.PHONY: all rebuild clean
