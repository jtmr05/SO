# ----------//----------
# This is a C makefile.|
# ----------//----------


#define the source, header, bin and object files directories
SRC_DIR := src
BIN_DIR := bin
HDR_DIR := include
OBJ_DIR := obj

#define the compiler
CC := gcc

#compiler flags
FLAGS := -I$(HDR_DIR) -Wall -Wextra -Wsign-conversion



#define the source and object files, and executable
SRC_FILES := $(shell find $(SRC_DIR) -name *.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(patsubst %.c,%.o,$(SRC_FILES)))
BIN := $(BIN_DIR)/program



#make default goal (using make with no specified recipe)
.DEFAULT_GOAL := $(BIN)

build: clean $(BIN)

$(BIN): $(OBJ_FILES)
	$(CC) $(FLAGS) $^ -o $@

#generate each object file according to the corresponding source file
#create directories as needed
$(OBJ_FILES): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -o $@





#'clean', 'run' and 'init' don't represent actual file generating recipes
.PHONY: clean run init

clean:
	-rm -rf $(OBJ_DIR)/*
	-rm -rf $(BIN_DIR)/*
	-rm a.sh b.sh c.sh nums.txt

run:
	@$(BIN)

init:
	-mkdir $(SRC_DIR)
	-mkdir $(HDR_DIR)
	-mkdir $(OBJ_DIR)
	-mkdir $(BIN_DIR)