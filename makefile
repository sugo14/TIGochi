# SRC = main.c
# EXE = main
# LIB = -lm

# OBJ = $(SRC:.c=.o)

# main:
# 	gcc $(SRC) -o $(EXE) $(LIB)

# clean:
# 	rm -f $(OBJ) $(EXE)

# ----------------------------
# Makefile Options
# ----------------------------

NAME = TIGotchi
# ICON = icon.png # i dont have an icon rn
DESCRIPTION = "TI-84 CE Tamagotchi"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# modified settings
ARCHIVED = YES # stores in archive instead of ram
PREFER_OS_CRT = NO # might make program a little smaller
HAS_PRINTF = NO # -8 kb program size

# ----------------------------

include $(shell cedev-config --makefile)
