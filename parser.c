#include <string.h>
#include <stdlib.h>
#include "parser.h"

Command parseCommand(char *input) {
    Command cmd = {1, {0}, 0, {0}};
    int i = 0;

    // parse optional count
    if (input[i] >= '1' && input[i] <= '9') {
        cmd.count = 0;
        while (input[i] >= '0' && input[i] <= '9')
            cmd.count = cmd.count * 10 + (input[i++] - '0');
    }

    // parse verb (1-3 chars, letters only)
    int v = 0;
    while (input[i] && input[i] >= 'a' && input[i] <= 'z' && v < 3)
        cmd.verb[v++] = input[i++];
    cmd.verb[v] = '\0';

    // parse optional modifier (hjkl or other single char)
    if (input[i]) cmd.modifier = input[i];

    return cmd;
}