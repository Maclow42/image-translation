# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

SRC = main.c ./src/Matrix/matrix.c ./src/NeuronalNetwork/trainNetwork.c ./src/BuildDataImage/buildDataImage.c ./src/SaveParams/saveParams.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

main: ${OBJ}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

run: main
	./main

# END
