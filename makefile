CC=g++
CFLAGS=-c -g -Wall
LDFLAGS= -g
SOURCES=scanner.cc token.cc tree.cc parser.cc main.cc StandardizeAST.cc cse_machine.cc delta.cc environment.cc cse_nodes.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=p1

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o p1
