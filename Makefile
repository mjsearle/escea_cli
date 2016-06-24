#Makefile for escea

CC=g++
CFLAGS=-c
SOURCES=$(wildcard src/*.cpp)
HEADERS=$(wildcard src/*.hpp)
OBJECTS=$(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))

all:  escea

escea: $(OBJECTS)
	$(CC) $(OBJECTS) -o escea
obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf obj/*.o escea
