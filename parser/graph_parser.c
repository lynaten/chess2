#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct GraphNode {
    char* name;
    struct GraphNode** calls;
    int calls_count;
    struct GraphNode** called_by;
    int called_by_count;
    int visited;
    int total_value;
    int value[8][8];
} GraphNode;

GraphNode** all_nodes = NULL;
int node_count = 0;

GraphNode* find_or_create_node(const char* name) {
    for (int i = 0; i < node_count; i++) {
        if (strcmp(all_nodes[i]->name, name) == 0) {
            return all_nodes[i];
        }
    }

    GraphNode* new_node = (GraphNode*)malloc(sizeof(GraphNode));
    if (!new_node) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }

    new_node->name = strdup(name);
    if (!new_node->name) {
        perror("Failed to allocate memory for node name");
        exit(EXIT_FAILURE);
    }
    new_node->calls = NULL;
    new_node->calls_count = 0;
    new_node->called_by = NULL;
    new_node->called_by_count = 0;
    new_node->visited = 0;

    all_nodes = (GraphNode**)realloc(all_nodes, (node_count + 1) * sizeof(GraphNode*));
    if (!all_nodes) {
        perror("Failed to reallocate memory for all_nodes array");
        exit(EXIT_FAILURE);
    }
    all_nodes[node_count++] = new_node;

    return new_node;
}

void add_connection(GraphNode* from, GraphNode* to) {
    int calls_exists = 0;
    for (int i = 0; i < from->calls_count; i++) {
        if (from->calls[i] == to) {
            calls_exists = 1;
            break;
        }
    }
    if (!calls_exists) {
        from->calls = (GraphNode**)realloc(from->calls, (from->calls_count + 1) * sizeof(GraphNode*));
        if (!from->calls) {
            perror("Failed to reallocate memory for calls array");
            exit(EXIT_FAILURE);
        }
        from->calls[from->calls_count++] = to;
    }

    int called_by_exists = 0;
    for (int i = 0; i < to->called_by_count; i++) {
        if (to->called_by[i] == from) {
            called_by_exists = 1;
            break;
        }
    }
    if (!called_by_exists) {
        to->called_by = (GraphNode**)realloc(to->called_by, (to->called_by_count + 1) * sizeof(GraphNode*));
         if (!to->called_by) {
            perror("Failed to reallocate memory for called_by array");
            exit(EXIT_FAILURE);
        }
        to->called_by[to->called_by_count++] = from;
    }
}


void trim_whitespace(char* str) {
    if (!str) return;
    char* start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }

    memmove(str, start, strlen(start) + 1);

    if (*str == 0)
        return;

    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    *(end + 1) = 0;
}


void parse_connections(GraphNode* current_node, char* line, int is_calls_line) {
    char* token = strtok(line, ",");
    while (token != NULL) {
        trim_whitespace(token);
        if (strlen(token) > 0) {
            GraphNode* other_node = find_or_create_node(token);
            if (is_calls_line) {
                add_connection(current_node, other_node);
            } else {
                add_connection(other_node, current_node);
            }
        }
        token = strtok(NULL, ",");
    }
}

void dfs_topological_sort(GraphNode* node) {
    if (node->visited == 2) {
        return;
    }
    if (node->visited == 1) {
        return;
    }

    node->visited = 1;

    for (int i = 0; i < node->calls_count; i++) {
        dfs_topological_sort(node->calls[i]);
    }

    node->visited = 2;
    printf("%s\n", node->name);
}


void free_graph() {
    for (int i = 0; i < node_count; i++) {
        free(all_nodes[i]->name);
        free(all_nodes[i]->calls);
        free(all_nodes[i]->called_by);
        free(all_nodes[i]);
    }
    free(all_nodes);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[1024];
    GraphNode* current_node = NULL;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "*", 1) == 0 && strcmp(line, "*end") != 0) {
            char* node_name = line + 1;
            trim_whitespace(node_name);
            current_node = find_or_create_node(node_name);
        } else if (strstr(line, "calls:") == line) {
            if (current_node) {
                char* connections_str = line + strlen("calls:");
                parse_connections(current_node, connections_str, 1);
            }
        } else if (strstr(line, "called by:") == line) {
             if (current_node) {
                char* connections_str = line + strlen("called by:");
                parse_connections(current_node, connections_str, 0);
            }
        }
    }

    fclose(file);

    GraphNode* start_node = NULL;
    for (int i = 0; i < node_count; i++) {
        if (strcmp(all_nodes[i]->name, "Main evaluation") == 0) {
            start_node = all_nodes[i];
            break;
        }
    }

    if (start_node) {
        printf("\n--- Function Evaluation Order (Topological Sort) ---\n");
        dfs_topological_sort(start_node);
    } else {
        fprintf(stderr, "\nWarning: Start node 'Main evaluation' not found. Cannot perform sort.\n");
    }

    free_graph();

    return EXIT_SUCCESS;
}
