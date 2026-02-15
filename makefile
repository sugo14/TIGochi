SRC = main.c
EXE = main

OBJ = $(SRC:.c=.o)

main:
	gcc $(SRC) -o $(EXE)

clean:
	rm -f $(OBJ) $(EXE)
