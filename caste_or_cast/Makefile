CXX ?= g++
CXXFLAGS= -std=c++20 -Wall -Wextra -Werror -pedantic \
		  -Wold-style-cast

OBJS = ant.o nurturer.o provider.o queen.o colony.o main_example.o

all: my_colony

my_colony: $(OBJS)
	$(CXX) -o $@ $^

clean:
	$(RM) $(OBJS) my_colony
.PHONY:
	clean
