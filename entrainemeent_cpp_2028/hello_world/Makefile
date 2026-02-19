CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -Werror -Wold-style-cast -pedantic

OBJ=hello_world.o
TARGET=hello_world
SOURCE=hello_world.cc

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %c
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	$(RM) $(TARGET) $(OBJ)

