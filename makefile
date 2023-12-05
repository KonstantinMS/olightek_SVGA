CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=register_limits.cpp display.cpp olightek.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=init

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


	