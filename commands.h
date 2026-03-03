#ifndef COMMANDS_H
#define COMMANDS_H
#include "reservoir.h"

void zoom();
int getIndexByID(int id);
void highlighter(int id, bool highlight);
//this needs to be defined first becasue its caleld by selectatcursor
void addSelection(int id);
void selectAtCursor(int y, int x);
void clearSelections();
void adoptEntry();


//VERY IMPORTANT LINES THEY REFER TO THE ENTRIES IN RESOIVOIR
// This is what we need to do for variables. we do extern so it dosent create its own copy of the array either.
extern int selections[64];
extern int selectionCount;

#endif