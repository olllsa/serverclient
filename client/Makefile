CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lpthread
SOURCES=client.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=client

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o client
