SRC = main.c
EXE = main
LIB = -lm

OBJ = $(SRC:.c=.o)

main:
	gcc $(SRC) -o $(EXE) $(LIB)

clean:
	rm -f $(OBJ) $(EXE)
