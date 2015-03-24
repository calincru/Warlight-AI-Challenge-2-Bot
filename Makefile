CPP = g++
FLAGS = -std=c++14 -Wall -Wextra -g
NAME = ab_bot

SRC = $(wildcard src/*.cpp) $(wildcard ./src/strategies/*.cpp)
HEADERS = $(wildcard src/*.h)
INCLUDE_PATH = ./include/
OBJ = $(subst src/,obj/,$(patsubst %.cpp,%.o,$(SRC)))

.PHONY: build run clean

build: $(OBJ)
	$(CPP) $(FLAGS) $^ -o $(NAME)

obj/%.o: src/%.cpp $(HEADERS)
	@mkdir -p obj
	@mkdir -p obj/strategies
	$(CPP) -c $(FLAGS) -I$(INCLUDE_PATH) $< -o $@ -I./src/strategies -I./src

run: build
	./$(NAME)

clean:
	rm -rf obj $(NAME)
