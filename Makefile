# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I.

# Directories
BUILD_DIR = build

# Source files
SRCS = main.c \
       fileio/fileio.c \
       encryption/encryption.c \
       encryption/algorithms.c \
       decryption/decryption.c \
       decryption/algorithms.c

# Object files (place them in the build directory, preserving structure)
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Target executable (place it in the build directory)
TARGET_NAME = fet
TARGET = $(BUILD_DIR)/$(TARGET_NAME)

# Default rule
all: $(TARGET)

# Linking rule: Create the final executable from object files
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $^

# Compilation rule: Create object files in the build directory from source files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup rule: Remove the entire build directory
clean:
	rm -rf $(BUILD_DIR)

# Phony target for clean to avoid conflicts with a file named 'clean'
.PHONY: all clean