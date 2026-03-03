#ifndef PARSER_H
#define PARSER_H

typedef struct {
    int count;
    char verb[4];
    char modifier;
    char prompt[256];
} Command;

Command parseCommand(char *input);

#endif