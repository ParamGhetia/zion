#include <ncurses.h>
#include "commands.h"
#include <stdbool.h>

bool normalMode = false;
int ch = 0;

// return codes
#define INPUT_PRINTABLE  1
#define INPUT_MOUSE      2
#define INPUT_ESC        3
#define INPUT_COMMAND    4
#define INPUT_OTHER      0

int interpretInput(int ch) {
    if (ch == 27) return INPUT_ESC;
    if (ch == KEY_MOUSE) return INPUT_MOUSE;
    if (normalMode && ch >= 32 && ch <= 126) return INPUT_COMMAND;
    if (!normalMode && ch >= 32 && ch <= 126) return INPUT_PRINTABLE;
    return INPUT_OTHER;
}

void initialization() {
    initscr();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    printf("\033[?1003h\n");
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

int main() {
    MEVENT event;
    initialization();

    while ((ch = getch()) != 'q') {
        int input = interpretInput(ch);

        switch (input) {
            case INPUT_PRINTABLE:
                printw("%c", ch);
                break;
            case INPUT_MOUSE:
                if (getmouse(&event) == OK)
                    move(event.y, event.x);
                break;
            case INPUT_ESC:
                normalMode = !normalMode;
                break;
            case INPUT_COMMAND:
                if (ch == '[') zoom();
                break;
        }

        refresh();
    }

    printf("\033[?1003l\n");
    endwin();
    return 0;
}
