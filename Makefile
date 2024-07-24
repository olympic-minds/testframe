# Compiler
CXX = g++

CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

SRCS = testframe.cpp graph.cpp utils.cpp matrix.cpp gen_utils.cpp

TARGET = program
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
