TARGET = grafos
LIBS = -lm
CC = g++
CFLAGS = -std=c++98 -g

.PHONY: default all clean

default: $(TARGET)
all: default

SOURCES = ejercicio_elfos.cpp list.cpp
OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = nodo.h list.h

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(TARGET)

clean:
	-rm -f *.o core $(TARGET)