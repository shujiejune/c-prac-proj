# --- Variables ---
CC = gcc
CFLAGS = -Wall -g
EXECUTABLE = calculator
SOURCE = calculator.c

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
	# Prompts for input and runs the calculator
	./$(EXECUTABLE)

# Target to clean up the directory.
# Usage: make clean
clean:
	rm -f $(EXECUTABLE)
