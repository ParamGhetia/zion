#ifndef RESERVOIR_H
#define RESERVOIR_H

typedef struct {
    int y, x;
    char text[1024];
    int entryID;
    int parentID; 
} Entry;

//these need to be here Idk how it was able to work before without it being defiend
extern Entry entries[1024];
extern int entryCount;

void reservoirDump(int startY, int startX, char *buffer, int *bufLen);
int searchEntry(int y, int x);
void loadReservoir();
void saveReservoir();
void displayReservoir();

#endif