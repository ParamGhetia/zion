CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LIBS = -lncurses -ltinfo

zion: zion.c commands.c reservoir.c parser.c
	$(CC) $(CFLAGS) zion.c commands.c reservoir.c parser.c $(LIBS) -o zion