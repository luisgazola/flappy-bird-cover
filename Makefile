TARGET = flappy

CXX = g++

CXXFLAGS = -std=c++17 \
            -Wall \
            -Wextra \
            -Wpedantic \
            -g \
            $(shell pkg-config --cflags sfml-graphics)

LDFLAGS = $(shell pkg-config --libs sfml-graphics)

SRC = main.cpp flappy.cpp

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean run