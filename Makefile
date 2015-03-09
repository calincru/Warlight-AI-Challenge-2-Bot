CPP = g++
FLAGS = -std=c++14 -Wall -Wextra -g
NAME = ab_bot

SRC = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h)
OBJ = $(subst src/,obj/,$(patsubst %.cpp,%.o,$(SRC)))

.PHONY: build run clean

build: $(OBJ)
	$(CPP) $(FLAGS) $^ -o $(NAME)

obj/%.o: src/%.cpp $(HEADERS)
	@mkdir -p obj
	$(CPP) -c $(FLAGS) $< -o $@

run: build
	./$(NAME)

clean:
	rm -rf obj $(NAME)
