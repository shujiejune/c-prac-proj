# --- Variables ---
CC = gcc
CFLAGS = -Wall -g
EXECUTABLE = dll_test
SOURCE = dll.c

# --- Targets ---

# Default target: Compiles the program.
# Usage: make
all: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXECUTABLE)

# Target to run the program.
# Usage: make run
run: $(EXECUTABLE)
	# Runs the Doubly Linked List test program
	./$(EXECUTABLE)

# Target to clean up the directory.
# Usage: make clean
clean:
	rm -f $(EXECUTABLE)
