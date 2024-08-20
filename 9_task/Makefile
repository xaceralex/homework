CC := gcc
SRC := part2.c
CLN := part2.i
SCF := part2.s
OBJ := part2.o
BIN := part2
FILE_TX := output.txt

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

.PHONY: clear
.PHONY: run
.PHONY: make2

$(BIN): $(SRC) $(CLN) $(SCF) $(OBJ)
		$(CC) -E $(SRC) -o $(CLN)$ 
		$(CC) -S $(CLN) -o $(SCF)$ 
		$(CC) -c $(SCF) 
		$(CC) $(OBJ) -o $(BIN)

$(CLN): $(SRC)

$(SCF): $(CLN)

$(OBJ): $(SCF)

$(BIN): $(OBJ)

$(LIB): $(LIB_SRC)
clear:
		rm -rf $(BIN) $(OBJ) $(SCF) $(CLN) $(LIB_OB) $(FILE_TX)
run:
		./$(BIN)$(PART)
make2:
	$(CC) part2.c -o part2 -lpanel -lncurses
