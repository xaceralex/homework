CC := gcc
SRC := part1.c
CLN := part1.i
SCF := part1.s
OBJ := part1.o
BIN := part1

.PHONY: clear
.PHONY: gdb

$(BIN): $(SRC) $(CLN) $(SCF) $(OBJ)
		$(CC) -E $(SRC) -o $(CLN)
		$(CC) -S $(CLN) -o $(SCF)
		$(CC) -c $(SCF) 
		$(CC) $(OBJ) -o $(BIN)

$(CLN): $(SRC)

$(SCF): $(CLN)

$(OBJ): $(SCF)

$(BIN): $(OBJ)

clear:
		rm -rf $(BIN) $(OBJ) $(SCF) $(CLN)

gdb:
		$(CC) -E $(SRC) -o $(CLN)
		$(CC) -S $(CLN) -o $(SCF)
		$(CC) -c -g $(SCF)
		$(CC) $(OBJ) -o $(BIN)
