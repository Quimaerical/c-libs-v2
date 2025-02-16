TARGET = RUN
LIBS = -lm
CC = g++
CFLAGS = -g

.PHONY: default all clean

default: $(TARGET)
all: default
OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h, *.hpp)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o core
	-rm RUN

cleanall: clean
	-rm -f $(TARGET)