SRC = src/board.cpp src/pain.cpp src/movegen.cpp src/test.cpp
OBJ = $(SRC:.cpp=.o)


pain: $(OBJ)
	g++ $(OBJ) -o pain

%.o: %.cpp
	g++ -std=c++20 -Iinclude -c $< -o $@

all : pain

clean :
	rm -f $(OBJ) pain