# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -I./include
LDFLAGS =
DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target executable
TARGET = $(BIN_DIR)/mini-shell

# Default target
all: $(TARGET)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking $@..."
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "Build successful! Run with: ./$(TARGET)"

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)
	@echo "Debug build complete!"

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: clean $(TARGET)
	@echo "Release build complete!"

# Run the shell
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete!"

# Clean and rebuild
rebuild: clean all

# Install (copy to /usr/local/bin)
install: $(TARGET)
	@echo "Installing mini-shell to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete! Run with: mini-shell"

# Uninstall
uninstall:
	@echo "Uninstalling mini-shell..."
	sudo rm -f /usr/local/bin/mini-shell
	@echo "Uninstall complete!"

# Check for memory leaks with valgrind
valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Static analysis with cppcheck
check:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SRC_DIR)

# Format code with clang-format
format:
	clang-format -i $(SRC_DIR)/*.c $(INC_DIR)/*.h

# Show help
help:
	@echo "Mini Shell - Makefile targets:"
	@echo "  make          - Build the project (default)"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make release  - Build optimized release version"
	@echo "  make run      - Build and run the shell"
	@echo "  make clean    - Remove build files"
	@echo "  make rebuild  - Clean and rebuild"
	@echo "  make install  - Install to /usr/local/bin"
	@echo "  make uninstall- Remove from /usr/local/bin"
	@echo "  make valgrind - Run with valgrind memory checker"
	@echo "  make check    - Run static analysis"
	@echo "  make format   - Format code with clang-format"
	@echo "  make help     - Show this help message"

.PHONY: all debug release run clean rebuild install uninstall valgrind check format help
