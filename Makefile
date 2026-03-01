CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LIBS = -lncurses -ltinfo

zion: zion.c commands.c
	$(CC) $(CFLAGS) zion.c commands.c reservoir.c $(LIBS) -o zion