#ifndef COMMANDS_H
#define COMMANDS_H
#include "reservoir.h"

void zoom();
void selectAtCursor(int y, int x);

//VERY IMPORTANT LINES THEY REFER TO THE ENTRIES IN RESOIVOIR
// This is what we need to do for variables. we do extern so it dosent create its own copy of the array either.
extern Entry entries[1024];
extern int entryCount; 

#endif