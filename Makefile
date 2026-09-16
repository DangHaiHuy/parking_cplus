CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

SRC := $(wildcard src/*.cpp src/*/*.cpp)
OBJ := $(SRC:.cpp=.o)
TARGET := smart_parking

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): main.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(OBJ)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: tests/test_main.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) -o tests/run_tests tests/test_main.cpp $(OBJ)
	./tests/run_tests

clean:
	rm -f src/*.o src/*/*.o $(TARGET) tests/run_tests
