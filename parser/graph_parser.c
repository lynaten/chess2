#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct GraphNode
{
    char *name;
    struct GraphNode **calls;
    int calls_count;
    struct GraphNode **called_by;
    int called_by_count;
    int visited;
    int total_value;
    int value[8][8];
} GraphNode;
