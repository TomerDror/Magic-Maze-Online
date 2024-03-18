# PROGRAMS := backend/main extras/SqlFieldPieceMaker
PROGRAMS := backend/main extras/SqlFieldPieceMaker
# Directory paths
SRCDIR := ../src
EXEDIR := ../exes
# Compiler and flags
CC := g++
CFLAGS := -Wall -Wextra
LDFLAGS := -lws2_32 -lsqlite3

# Rule to compile each program
$(PROGRAMS): %: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -o $(EXEDIR)/$@ $< $(LDFLAGS)

# Rule to compile backend/main with additional source files
backend/main: $(SRCDIR)/backend/main.cpp $(SRCDIR)/backend/PreFieldPiece.cpp $(SRCDIR)/backend/FieldPiece.cpp
	$(CC) $(CFLAGS) -o $(EXEDIR)/$@ $^ $(LDFLAGS)

# Rule to compile all programs
all: $(PROGRAMS)

# Rule to clean the executables
clean:
	rm -f $(EXEDIR)/*

#  g++ -Wall -Wextra -o ../exes/backend/main ../src/backend/main.cpp ../src/backend/Player.cpp ../src/backend/Field.cpp ../src/backend/Character.cpp ../src/backend/PreFieldPiece.cpp ../src/backend/FieldPiece.cpp ../src/backend/Tile.cpp -lws2_32 -lsqlite3
