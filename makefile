# ----------------------------
# CEdev Options
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
# HAS_PRINTF = NO # -8 kb program size # ! OMG THIS LINE IS THE DEVIL

# ----------------------------
# CEdev Build
# ----------------------------

include $(shell cedev-config --makefile)

# ----------------------------
# Laptop Build
# ----------------------------

laptop:
	gcc src/main.c -o main -lm

laptop-clean:
	rm -f main
