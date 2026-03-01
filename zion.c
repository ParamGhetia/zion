#include <ncurses.h>
#include "commands.h"
#include "reservoir.h"
#include <stdbool.h>
#include <string.h>

bool normalMode = false;
int ch = 0;

// return codes
#define OPERATION_QUIT   1
#define INPUT_PRINTABLE  2
#define INPUT_DELETE     3
#define INPUT_MOUSE      4
#define INPUT_ESC        5
#define COMMAND_ZOOM     6
#define COMMAND_SELECT   7
#define INPUT_OTHER      0

//standard convention for defining ctrl+k input. Just k with mask off of bits.
#define CTRL(k) ((k) & 0x1f)

// the buffer that accumulates what you type
char buffer[1024] = {0};
int bufLen = 0;
int startX = 0, startY = 0;  // position where typing began
int input = 0;

void splashScreen() {
    FILE *f = fopen("splash.txt", "r");
    if (!f) return;
    char line[256];
    while (fgets(line, sizeof(line), f))
        printw("%s", line);
    fclose(f);
    refresh();
    getch();      // wait for any key
    clear();
    refresh();
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
    if (ch == CTRL('q'))                                   return OPERATION_QUIT;
    if (ch == 27)                                          return INPUT_ESC;
    if (ch == KEY_MOUSE)                                   return INPUT_MOUSE;
    if (normalMode && ch == 91)                            return COMMAND_ZOOM;
    if (!normalMode && ch >= 32 && ch <= 126)              return INPUT_PRINTABLE;
    if (!normalMode && (ch == 127 || ch == KEY_BACKSPACE)) return INPUT_DELETE;
    if (normalMode && (ch == 115))                         return COMMAND_SELECT;
    return INPUT_OTHER;
}

int main() {
    MEVENT event;
    int y, x;
    initialization();
    splashScreen();
    loadReservoir();
    displayReservoir();

    while ((input = interpretInput(ch = getch())) != OPERATION_QUIT) {

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
                    getyx(stdscr, y, x);
                    mvprintw(y, x-1, " ");
                    move(y, x-1);
                }
                break;

            case INPUT_ESC:
                if (!normalMode && bufLen > 0)
                    reservoirDump(startY, startX, buffer, &bufLen);
                normalMode = !normalMode;
                break;

            case INPUT_MOUSE:
                if (getmouse(&event) == OK) {
                    if (!normalMode && bufLen > 0)
                        reservoirDump(startY, startX, buffer, &bufLen);
                    move(event.y, event.x);
                }
                break;

            case COMMAND_ZOOM:
                zoom();
                break;
            
            case COMMAND_SELECT:
                getyx(stdscr, y, x);
                selectAtCursor(y, x);
                break;
        }

        refresh();
    }
    saveReservoir();

    printf("\033[?1003l\n");
    endwin();
    return 0;
}
// need to figure out how to make it so it prints onto screen from text file. 
// ALso implement thing where it sorts the text file when you press q. 
// next need to figure out how to allow for ctrl moving stuff

//ideally this would replace my scratchpad and like i can have a daily text file
// I would ujst have keyboard shortcut to automatically open the scratpad whenever I want
//it would be fun to make sound effects for it like using audcacity but i should make them all vocally that would be funny
//you can also have empty text file in which case it will print a cool ascii thing pertaining to the matrix and then on any input it will erase that 