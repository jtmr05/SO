# ----------//----------
# This is a C makefile.|
# ----------//----------


#directories
SRC_DIR 		:= src
BIN_DIR 		:= bin
INC_DIR 		:= include
RSR_DIR 		:= resources

#compiler
CC 				:= gcc




#files
SRC_FILES 		:= $(shell find $(SRC_DIR) -name *.c)
BIN_FILES 		:= $(patsubst $(SRC_DIR)/%,$(BIN_DIR)/%,$(patsubst %.c,%,$(SRC_FILES)))


#compiler flags
FLAGS 			:= -I$(INC_DIR) -Wall -Wextra -Wsign-conversion




#make default goal (using make with no specified recipe)
.DEFAULT_GOAL 	:= all

all: $(BIN_FILES)

build: clean all

$(BIN_FILES): $(BIN_DIR)/% : $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $^ -o $@



#'clean' and 'init' don't represent actual file generating recipes
.PHONY: clean init

clean:
	-rm -rf $(BIN_DIR)

init:
	-mkdir $(SRC_DIR)
	-mkdir $(INC_DIR)
	-mkdir $(BIN_DIR)
	-mkdir $(RSR_DIR)