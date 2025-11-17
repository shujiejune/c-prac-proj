# --- Variables ---

# Compiler
CC = gcc

# Compiler flags:
# -Wall = Show all warnings
# -g    = Include debug symbols
CFLAGS = -Wall -g

# Linker flags:
# -lncurses = Link the ncurses library
LDFLAGS = -lncurses

# Source file
SOURCES = flappy_bird.c

# Executable file name
EXECUTABLE = flappy_bird

# --- Targets ---

# Default target: Build the executable
# Executed when you just type: make
all: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)

# Target to run the program
# This is the "1 line" command you asked for.
# Type: make run
run: all
	./$(EXECUTABLE)

# Target to clean up the directory
# Removes the compiled executable
# Type: make clean
clean:
	rm -f $(EXECUTABLE)
