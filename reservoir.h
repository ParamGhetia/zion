#ifndef RESERVOIR_H
#define RESERVOIR_H

typedef struct {
    int y, x;
    char text[1024];
    int entryID;
    int parentID; 
} Entry;

void reservoirDump(int startY, int startX, char *buffer, int *bufLen);
int searchEntry(int y, int x);
void loadReservoir();
void saveReservoir();
void displayReservoir();

#endif