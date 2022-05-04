# ----------//----------
# This is a C makefile.|
# ----------//----------


#directories
SRC_DIR 		:= src
BIN_DIR 		:= bin
INC_DIR 		:= include
OBJ_DIR 		:= obj
RSR_DIR 		:= resources

#compiler
CC 				:= gcc




#files
SRC_FILES 		:= $(shell find $(SRC_DIR) -name *.c)
OBJ_FILES 		:= $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(patsubst %.c,%.o,$(SRC_FILES)))
BIN 			:= $(BIN_DIR)/a.out


#compiler flags
FLAGS 			:= -I$(INC_DIR) -Wall -Wextra -Wsign-conversion




#make default goal (using make with no specified recipe)
.DEFAULT_GOAL 	:= all

all: $(BIN)

build: clean all

$(BIN): $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $^ -o $@

#generate each object file according to the corresponding source file
#create directories as needed
$(OBJ_FILES): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -o $@





#'clean', 'run' and 'init' don't represent actual file generating recipes
.PHONY: clean run init

clean:
	-rm -rf $(OBJ_DIR)
	-rm -rf $(BIN_DIR)

run:
	@$(BIN)

init:
	-mkdir $(SRC_DIR)
	-mkdir $(INC_DIR)
	-mkdir $(OBJ_DIR)
	-mkdir $(BIN_DIR)
	-mkdir $(RSR_DIR)