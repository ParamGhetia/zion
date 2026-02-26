#include <ncurses.h>
#include "commands.h"

void zoom() {
    int top = 0, left = 0, bot = LINES-1, right = COLS-1;

    while (1) {
        // draw corners of current quadrant
        mvaddch(top, left, (chtype)219);
        mvaddch(top, right, (chtype)219);
        mvaddch(bot, left, (chtype)219);
        mvaddch(bot, right, (chtype)219);
        move(top, left);
        refresh();

        int midY = (top + bot) / 2;
        int midX = (left + right) / 2;

        switch(getch()) {
            case 'e': bot = midY; right = midX; break; // top-left
            case 'y': bot = midY; left = midX;  break; // top-right
            case 'c': top = midY; right = midX; break; // bot-left
            case 'b': top = midY; left = midX;  break; // bot-right
            default: return; // exit zoom on anything else
        }
    }
}