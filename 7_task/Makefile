CC := gcc
SRC := main.c
LIB_SRC := add.c sub.c mul.c div1.c
LIB_OB := add.o sub.o mul.o div1.o
LIB := func
CLN := main.i
SCF := main.s
OBJ := main.o
BIN := main

.PHONY: clear
.PHONY: gdb

$(BIN): $(SRC) $(CLN) $(SCF) $(OBJ) $(LIB)
		$(CC) $(LIB_SRC) -c $(LIB_OB)
		ar rc  lib$(LIB).a $(LIB_OB)
		$(CC) $(SRC) -c $(OBJ)
		$(CC) $(OBJ) -o $(BIN) -L. -l$(LIB)

$(CLN): $(SRC)

$(SCF): $(CLN)

$(OBJ): $(SCF)

$(BIN): $(OBJ)

$(LIB): $(LIB_SRC)
clear:
		rm -rf $(BIN) $(OBJ) $(SCF) $(CLN) $(LIB_OB) lib$(LIB).a 

gdb:
		$(CC) -E $(SRC) -o $(CLN)
		$(CC) -S $(CLN) -o $(SCF)
		$(CC) -c -g $(SCF)
		$(CC) $(OBJ) -o $(BIN)
