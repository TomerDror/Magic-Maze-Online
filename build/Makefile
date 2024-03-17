# PROGRAMS := tests/Client tests/Server tests/SqlImporter tests/PythonRunner tests/ThreadTest
PROGRAMS :=  backend/main extras/SqlFieldPieceMaker# extras/SqlFieldPieceReader
SRCDIR := ../src
EXEDIR := ../exes
CC := g++
CFLAGS := -Wall -Wextra  
LDFLAGS := -lws2_32 -lsqlite3

all: $(PROGRAMS)

# Rule to compile each program
$(PROGRAMS): %: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -o $(EXEDIR)/$@ $< $(LDFLAGS)

clean:
	rm -f $(EXEDIR)/*
