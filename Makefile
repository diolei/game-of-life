CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lSDL2 -lm

SRCS = game.c

all: game

game: $(SRCS)
	 $(CC) $(CFLAGS) $(SRCS) -o game $(LDFLAGS)

clean:
	rm -f game


