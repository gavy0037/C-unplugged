# ============================
# 🎵  Makefile for C-Unplugged
# ============================

# Compiler and flags
CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11 -g -Iinclude

# Directories
SRC_DIR  := src
OBJ_DIR  := build
BIN_DIR  := bin

# Target executable name
TARGET   := $(BIN_DIR)/c_unplugged

# Find all source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Default target
all: dirs $(TARGET)

# Create required directories
dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link all object files into final executable
$(TARGET): $(OBJ_FILES)
	@echo "🔗 Linking..."
	$(CC) $(OBJ_FILES) -o $(TARGET)
	@echo "✅ Build successful! Executable: $(TARGET)"

# Compile each .c file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "🧩 Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: all
	@echo "🚀 Running C-Unplugged..."
	@$(TARGET)

# Debug build with additional flags
debug:
	$(MAKE) CFLAGS="$(CFLAGS) -DDEBUG -O0 -fsanitize=address" clean all

# Clean build and binary files
clean:
	@echo "🧹 Cleaning build files..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "✅ Clean complete."

# Help menu
help:
	@echo "=============================="
	@echo "🎵 C-Unplugged Build Commands"
	@echo "=============================="
	@echo "make            -> Build project"
	@echo "make run        -> Build & run executable"
	@echo "make debug      -> Build with sanitizer flags"
	@echo "make clean      -> Remove all build outputs"
	@echo "make help       -> Show this help menu"
	@echo "=============================="

.PHONY: all clean run debug help dirs
