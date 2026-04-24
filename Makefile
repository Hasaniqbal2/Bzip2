CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET   = bzip2_impl
SOURCES  = src/main.cpp src/block.cpp src/rle.cpp src/bwt.cpp src/config.cpp
INCLUDES = -Iinclude

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(SOURCES)

clean:
	rm -f $(TARGET) __test.*

test: all
	./$(TARGET) test test.txt config.ini

.PHONY: all clean test