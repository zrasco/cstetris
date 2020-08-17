# Makefile for tetris
# Zeb Rasco

CXX=g++

# Or you can use:
#CXX=clang++
#CXXFLAGS=-std=c++14 -Wall -Wno-c++98-compat -g

TARGET=tetris

all: Console.o Gamestate.o WindowWrapper.o Grid.o Pieces.o main.o
	$(CXX) $(CXXFLAGS) Console.o Gamestate.o WindowWrapper.o Grid.o Pieces.o main.o -o $(TARGET) -lncurses
Console.o: Console.cpp Console.h
	$(CXX) $(CXXFLAGS) -c Console.cpp
Gamestate.o: Gamestate.cpp Gamestate.h
	$(CXX) $(CXXFLAGS) -c Gamestate.cpp
WindowWrapper.o: WindowWrapper.cpp WindowWrapper.h
	$(CXX) $(CXXFLAGS) -c WindowWrapper.cpp
Grid.o: Grid.cpp Grid.h Pieces.h
	$(CXX) $(CXXFLAGS) -c Grid.cpp
Pieces.o: Pieces.cpp Pieces.h
	$(CXX) $(CXXFLAGS) -c Pieces.cpp
main.o: main.cpp WindowWrapper.h Gamestate.h Console.h Grid.h
	$(CXX) $(CXXFLAGS) -c main.cpp
clean:
	rm -f *.o $(TARGET)
