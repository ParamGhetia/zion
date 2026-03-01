#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "reservoir.h"

//Entry ID Pooling and creation
//There is a pool of used IDs that can be reused. If the pool is empty it will just create a new one.
int idPool[1024];
int idPoolCount = 0;
int nextID = 0;

int assignID() {
    if (idPoolCount > 0)
        //uses an ID from the pool
        return idPool[--idPoolCount]; 
    return nextID++;                 
}

void freeID(int id) {
    //this will return the current id to the pool and allow it to use it.
    idPool[idPoolCount++] = id;  
}

Entry entries[1024];
int entryCount = 0;

void reservoirDump(int startY, int startX, char *buffer, int *bufLen) {
    if (entryCount >= 1024) return;

    // start from back, shift entries right until we find the right spot
    int i = entryCount;
    while (i > 0) {
        Entry *prev = &entries[i-1];
        if (prev->y < startY || (prev->y == startY && prev->x < startX))
            break;
        entries[i] = entries[i-1];
        i--;
    }

    entries[i].y = startY;
    entries[i].x = startX;
    strncpy(entries[i].text, buffer, 1023);
    entries[i].text[1023] = '\0';
    entries[i].entryID = assignID();
    entries[i].parentID = -1;   
    entryCount++;

    memset(buffer, 0, *bufLen);
    *bufLen = 0;
}
/*
void deleteEntry
    freeID(entries[searchEntry(_______)].entryID);
*/

int searchEntry(int y, int x) {
    int lo = 0, hi = entryCount - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (entries[mid].y == y && entries[mid].x == x)
            return mid;
        if (entries[mid].y < y || (entries[mid].y == y && entries[mid].x < x))
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}

void loadReservoir() {
    FILE *f = fopen("reservoir.zion", "rb");
    if (!f) return;
    entryCount = fread(entries, sizeof(Entry), 1024, f);
    fclose(f);
}

void saveReservoir() {
    FILE *f = fopen("reservoir.zion", "wb");
    if (!f) return;
    fwrite(entries, sizeof(Entry), entryCount, f);
    fclose(f);
}

void displayReservoir() {
    for (int i = 0; i < entryCount; i++)
        mvprintw(entries[i].y, entries[i].x, "%s", entries[i].text);
}