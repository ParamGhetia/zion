#include <ncurses.h>
#include "commands.h"
#include <stdbool.h>
#include <string.h>

bool normalMode = false;
int ch = 0;

// return codes
#define INPUT_PRINTABLE  1
#define INPUT_DELETE     2
#define INPUT_MOUSE      3
#define INPUT_ESC        4
#define COMMAND_ZOOM     5
#define INPUT_OTHER      0

// the buffer that accumulates what you type
char buffer[1024] = {0};
int bufLen = 0;
int startX = 0, startY = 0;  // position where typing began

void reservoirDump() {
    FILE *f = fopen("reservoir.txt", "a");
    if (f) {
        fprintf(f, "%d,%d,%s\n", startY, startX, buffer);
        fclose(f);
    }
    // clear buffer
    memset(buffer, 0, sizeof(buffer));
    bufLen = 0;
}

void initialization() {
    initscr();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    printf("\033[?1003h\n");
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

int interpretInput(int ch) {
    if (ch == 27)                             return INPUT_ESC;
    if (ch == KEY_MOUSE)                      return INPUT_MOUSE;
    if (normalMode && ch >= 32 && ch <= 126)  return COMMAND_ZOOM;
    if (!normalMode && ch >= 32 && ch <= 126) return INPUT_PRINTABLE;
    if (!normalMode && (ch == 127 || ch == KEY_BACKSPACE)) return INPUT_DELETE;
    return INPUT_OTHER;
}

int main() {
    MEVENT event;
    initialization();

    while ((ch = getch()) != 'q') {
        int input = interpretInput(ch);

        switch (input) {
            case INPUT_PRINTABLE:
                // record start position on first character
                if (bufLen == 0)
                    getyx(stdscr, startY, startX);
                // add to buffer
                buffer[bufLen++] = (char)ch;
                buffer[bufLen] = '\0';
                printw("%c", ch);
                break;

            case INPUT_DELETE:
                if (bufLen > 0) {
                    bufLen--;
                    buffer[bufLen] = '\0';
                    int y, x;
                    getyx(stdscr, y, x);
                    mvprintw(y, x-1, " ");
                    move(y, x-1);
                }
                break;

            case INPUT_ESC:
                if (!normalMode && bufLen > 0)
                    reservoirDump();  // dump on ESC if we have something
                normalMode = !normalMode;
                break;

            case INPUT_MOUSE:
                if (getmouse(&event) == OK) {
                    if (!normalMode && bufLen > 0)
                        reservoirDump();
                    move(event.y, event.x);
                }
                break;

            case COMMAND_ZOOM:
                if (ch == '[') zoom();
                break;
        }

        refresh();
    }

    printf("\033[?1003l\n");
    endwin();
    return 0;
}