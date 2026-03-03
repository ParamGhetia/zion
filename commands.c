#include <ncurses.h>
#include <stdbool.h>
#include "reservoir.h"
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

int selections[64]; 
int selectionCount = 0;

//ALL SELECTIONS AND STUFF SHOULD BE MADE BASED ON ID NOT BASED ON INDEX OR POINTER BECAUSE THOSE CHANGE WITH CHANGES TO ARRAY
//in terms of storiung everything should be stored with id but for usage we use this function to convert to index
int getIndexByID(int id) {
    for (int i = 0; i < entryCount; i++)
        if (entries[i].entryID == id)
            return i;
    return -1;
}

void selectAtCursor(int y, int x) {
    int idx = searchEntry(y, x);
    if (idx == -1) return;
    addSelection(entries[idx].entryID);
}

void addSelection(int id) {
    for (int i = 0; i < selectionCount; i++) {
        if (selections[i] == id) {
            selections[i] = selections[--selectionCount];
            highlighter(id, false);
            return;
        }
    }
    if (selectionCount < 64) {
        selections[selectionCount++] = id;
        highlighter(id, true);
    }
}

void clearSelections() {
    for (int i = 0; i < selectionCount; i++)
        highlighter(selections[i], false);
    selectionCount = 0;
}

void highlighter(int id, bool highlight) {
    int idx = getIndexByID(id);
    if (idx == -1) return;
    if (highlight) attron(A_REVERSE);
    if (entries[idx].parentID != -1)
        mvprintw(entries[idx].y, entries[idx].x, "%c %s", 45, entries[idx].text);
    else
        mvprintw(entries[idx].y, entries[idx].x, "%s", entries[idx].text);
    if (highlight) attroff(A_REVERSE);
}

void adoptEntry() {
    if (selectionCount < 2) return;
    for (int i = 1; i < selectionCount; i++) {
        int idx = getIndexByID(selections[i]);
        int parentIdx = getIndexByID(selections[0]);
        if (idx == -1 || parentIdx == -1) continue;
        entries[idx].parentID = entries[parentIdx].entryID;
        mvprintw(entries[idx].y, entries[idx].x, "%c %s", 45, entries[idx].text);
    }
}