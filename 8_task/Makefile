CC := gcc
SRC := main.c
LIB_SRC := add.c sub.c mul.c div1.c
LIB_OB := add.o sub.o mul.o div1.o
LIB := func
CLN := main.i
SCF := main.s
OBJ := main.o
BIN := main

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

.PHONY: clear
.PHONY: run

$(BIN): $(SRC) $(LIB_SRC) $(LIB_OB)
		$(CC) $(LIB_SRC) -fPIC -c
		$(CC) -shared $(LIB_OB) -o lib$(LIB).so
		$(CC) $(SRC) -o $(BIN) -L. -l$(LIB)

$(CLN): $(SRC)

$(SCF): $(CLN)

$(OBJ): $(SCF)

$(BIN): $(OBJ)

$(LIB): $(LIB_SRC)
clear:
		rm -rf $(BIN) $(OBJ) $(SCF) $(CLN) $(LIB_OB) lib$(LIB).a lib$(LIB).so 
run:
		LD_LIBRARY_PATH=$PWD:$(LD_LIBRARY_PATH) ./$(BIN)
		#LD_LIBRARY_PATH= $(current_dir) ./$(BIN) 
		#LD_LIBRARY_PATH= $(current_dir):$(LD_LIBRARY_PATH) ./$(BIN)
