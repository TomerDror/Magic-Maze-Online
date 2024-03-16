# PROGRAMS := tests/Client tests/Server tests/SqlImporter tests/PythonRunner tests/ThreadTest
PROGRAMS :=   backend/Character#backend/Player backend/Field backend/Character backend/PreFieldPiece backend/Tile backend/FieldPiece
SRCDIR := ../src
EXEDIR := ../exes
CC := g++
CFLAGS := -Wall -Wextra  
LDFLAGS := -lws2_32

all: $(PROGRAMS)

# Rule to compile each program
$(PROGRAMS): %: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -o $(EXEDIR)/$@ $< $(LDFLAGS)

clean:
	rm -f $(EXEDIR)/*
