# Compiler
CC      := gcc
GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
GTK_LDFLAGS := $(shell pkg-config --libs gtk+-3.0)

CFLAGS += -g $(GTK_CFLAGS) -Iinclude
LDFLAGS += $(GTK_LDFLAGS)

# Directories
BUILD_DIR := build
SRC_DIR   := src

# Sources
SRC := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/app_context.c \
	$(SRC_DIR)/algorithms/dfs.c \
	$(SRC_DIR)/algorithms/hdfs.c \
	$(SRC_DIR)/algorithms/dlx.c \
	$(SRC_DIR)/generator/generator.c \
	$(SRC_DIR)/gui/gui_main.c \
	$(SRC_DIR)/gui/gui_grid.c \
	$(SRC_DIR)/gui/gui_result.c \
	$(SRC_DIR)/gui/gui_textview.c \
	$(SRC_DIR)/gui/gui_callbacks_generate.c \
	$(SRC_DIR)/gui/gui_callbacks_run.c \
	$(SRC_DIR)/gui/gui_callbacks_result.c \
	$(SRC_DIR)/gui/gui_dialog.c \
	$(SRC_DIR)/utils/board_utils.c \
	$(SRC_DIR)/utils/parsing.c \
	$(SRC_DIR)/utils/solver.c \
	$(SRC_DIR)/utils/validation.c 

# Object files
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/$(SRC_DIR)/%.o,$(SRC))

# Target
TARGET := $(BUILD_DIR)/sudoku_gui

# Default build
all: $(TARGET)

# Build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile target
$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compile objects
$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Run
run: all
	./$(TARGET)

# Clean
clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean run