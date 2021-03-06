CC=g++
CFLAGS=-c
LDFLAGS=-lpcap -lpthread
SOURCES=AirodumpMain.cpp Airodump.cpp mac.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=airodump

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
