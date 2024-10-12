CC := gcc
SRC := part2.c
CLN := part2.i
SCF := part2.s
OBJ := part2.o
BIN := part2

.PHONY: clear
.PHONY: run

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

run:
		./$(BIN)