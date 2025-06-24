#ifndef GRAPH_H
#define GRAPH_H

// These includes are typically needed for common C library functions used in graph operations
// _DEFAULT_SOURCE is a GCC extension, often not needed in headers but good for consistency
#define _DEFAULT_SOURCE
#include <stdio.h>
#include "header/chess/evaluation.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h> // For bool if your compiler needs it explicitly for bool

// Define the GraphNode structure
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
    int colorflip_total_value;
    int colorflip_value[8][8];
} GraphNode;

// Declare global variables
extern GraphNode **all_nodes;
extern int node_count;

// Declare function prototypes
GraphNode *find_or_create_node(const char *name);
void add_connection(GraphNode *from, GraphNode *to);
void trim_whitespace(char *str);
void parse_connections(GraphNode *current_node, char *line, int is_calls_line);
void dfs_topological_sort(GraphNode *node);
void free_graph();
void reset_graph();
int get_function_index(const char *name);
int initialize_graph_from_file();
int main_function_handler(const char *name, ChessPosition *pos, ChessPosition *pos2, Square *square, void *param, bool colorflipped);

#endif // GRAPH_H