#include "header/graph/graph.h"

GraphNode **all_nodes = NULL;
int node_count = 0;

GraphNode *find_or_create_node(const char *name)
{
    for (int i = 0; i < node_count; i++)
    {
        if (strcmp(all_nodes[i]->name, name) == 0)
        {
            return all_nodes[i];
        }
    }

    GraphNode *new_node = (GraphNode *)malloc(sizeof(GraphNode));
    if (!new_node)
    {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }

    new_node->name = strdup(name);
    if (!new_node->name)
    {
        perror("Failed to allocate memory for node name");
        exit(EXIT_FAILURE);
    }
    new_node->calls = NULL;
    new_node->calls_count = 0;
    new_node->called_by = NULL;
    new_node->called_by_count = 0;
    new_node->visited = 0;

    all_nodes = (GraphNode **)realloc(all_nodes, (node_count + 1) * sizeof(GraphNode *));
    if (!all_nodes)
    {
        perror("Failed to reallocate memory for all_nodes array");
        exit(EXIT_FAILURE);
    }
    all_nodes[node_count++] = new_node;

    return new_node;
}

void add_connection(GraphNode *from, GraphNode *to)
{
    int calls_exists = 0;
    for (int i = 0; i < from->calls_count; i++)
    {
        if (from->calls[i] == to)
        {
            calls_exists = 1;
            break;
        }
    }
    if (!calls_exists)
    {
        from->calls = (GraphNode **)realloc(from->calls, (from->calls_count + 1) * sizeof(GraphNode *));
        if (!from->calls)
        {
            perror("Failed to reallocate memory for calls array");
            exit(EXIT_FAILURE);
        }
        from->calls[from->calls_count++] = to;
    }

    int called_by_exists = 0;
    for (int i = 0; i < to->called_by_count; i++)
    {
        if (to->called_by[i] == from)
        {
            called_by_exists = 1;
            break;
        }
    }
    if (!called_by_exists)
    {
        to->called_by = (GraphNode **)realloc(to->called_by, (to->called_by_count + 1) * sizeof(GraphNode *));
        if (!to->called_by)
        {
            perror("Failed to reallocate memory for called_by array");
            exit(EXIT_FAILURE);
        }
        to->called_by[to->called_by_count++] = from;
    }
}

void trim_whitespace(char *str)
{
    if (!str)
        return;
    char *start = str;
    while (isspace((unsigned char)*start))
    {
        start++;
    }

    memmove(str, start, strlen(start) + 1);

    if (*str == 0)
        return;

    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
    {
        end--;
    }

    *(end + 1) = 0;
}

void parse_connections(GraphNode *current_node, char *line, int is_calls_line)
{
    char *token = strtok(line, ",");
    while (token != NULL)
    {
        trim_whitespace(token);
        if (strlen(token) > 0)
        {
            GraphNode *other_node = find_or_create_node(token);
            if (is_calls_line)
            {
                add_connection(current_node, other_node);
            }
            else
            {
                add_connection(other_node, current_node);
            }
        }
        token = strtok(NULL, ",");
    }
}

int get_function_index(const char *name)
{
    if (strcmp(name, "Main evaluation") == 0)
        return 0;
    else if (strcmp(name, "Middle game evaluation") == 0)
        return 1;
    else if (strcmp(name, "End game evaluation") == 0)
        return 2;
    else if (strcmp(name, "Phase") == 0)
        return 3;
    else if (strcmp(name, "Rule50") == 0)
        return 4;
    else if (strcmp(name, "Scale factor") == 0)
        return 5;
    else if (strcmp(name, "Tempo") == 0)
        return 6;
    else if (strcmp(name, "Piece value mg") == 0)
        return 7;
    else if (strcmp(name, "Psqt mg") == 0)
        return 8;
    else if (strcmp(name, "Imbalance total") == 0)
        return 9;
    else if (strcmp(name, "Pawns mg") == 0)
        return 10;
    else if (strcmp(name, "Pieces mg") == 0)
        return 11;
    else if (strcmp(name, "Mobility mg") == 0)
        return 12;
    else if (strcmp(name, "Threats mg") == 0)
        return 13;
    else if (strcmp(name, "Passed mg") == 0)
        return 14;
    else if (strcmp(name, "Space") == 0)
        return 15;
    else if (strcmp(name, "King mg") == 0)
        return 16;
    else if (strcmp(name, "Winnable total mg") == 0)
        return 17;
    else if (strcmp(name, "Piece value bonus") == 0)
        return 18;
    else if (strcmp(name, "Non pawn material") == 0)
        return 19;
    else if (strcmp(name, "Piece value eg") == 0)
        return 20;
    else if (strcmp(name, "Psqt bonus") == 0)
        return 21;
    else if (strcmp(name, "Psqt eg") == 0)
        return 22;
    else if (strcmp(name, "Imbalance") == 0)
        return 23;
    else if (strcmp(name, "Bishop pair") == 0)
        return 24;
    else if (strcmp(name, "Bishop count") == 0)
        return 25;
    else if (strcmp(name, "Opposite bishops") == 0)
        return 26;
    else if (strcmp(name, "Doubled isolated") == 0)
        return 27;
    else if (strcmp(name, "Isolated") == 0)
        return 28;
    else if (strcmp(name, "Backward") == 0)
        return 29;
    else if (strcmp(name, "Doubled") == 0)
        return 30;
    else if (strcmp(name, "Connected") == 0)
        return 31;
    else if (strcmp(name, "Connected bonus") == 0)
        return 32;
    else if (strcmp(name, "Weak unopposed pawn") == 0)
        return 33;
    else if (strcmp(name, "Blocked") == 0)
        return 34;
    else if (strcmp(name, "Pawns eg") == 0)
        return 35;
    else if (strcmp(name, "Pawn attacks span") == 0)
        return 36;
    else if (strcmp(name, "Supported") == 0)
        return 37;
    else if (strcmp(name, "Phalanx") == 0)
        return 38;
    else if (strcmp(name, "Candidate passed") == 0)
        return 39;
    else if (strcmp(name, "Opposed") == 0)
        return 40;
    else if (strcmp(name, "Rank") == 0)
        return 41;
    else if (strcmp(name, "Mobility area") == 0)
        return 42;
    else if (strcmp(name, "Outpost square") == 0)
        return 43;
    else if (strcmp(name, "Space area") == 0)
        return 44;
    else if (strcmp(name, "King proximity") == 0)
        return 45;
    else if (strcmp(name, "Passed block") == 0)
        return 46;
    else if (strcmp(name, "Passed rank") == 0)
        return 47;
    else if (strcmp(name, "Outpost total") == 0)
        return 48;
    else if (strcmp(name, "Minor behind pawn") == 0)
        return 49;
    else if (strcmp(name, "Bishop pawns") == 0)
        return 50;
    else if (strcmp(name, "Bishop xray pawns") == 0)
        return 51;
    else if (strcmp(name, "Rook on queen file") == 0)
        return 52;
    else if (strcmp(name, "Rook on king ring") == 0)
        return 53;
    else if (strcmp(name, "Bishop on king ring") == 0)
        return 54;
    else if (strcmp(name, "Rook on file") == 0)
        return 55;
    else if (strcmp(name, "Trapped rook") == 0)
        return 56;
    else if (strcmp(name, "Weak queen") == 0)
        return 57;
    else if (strcmp(name, "Queen infiltration") == 0)
        return 58;
    else if (strcmp(name, "King protector") == 0)
        return 59;
    else if (strcmp(name, "Long diagonal bishop") == 0)
        return 60;
    else if (strcmp(name, "Outpost") == 0)
        return 61;
    else if (strcmp(name, "Reachable outpost") == 0)
        return 62;
    else if (strcmp(name, "Pieces eg") == 0)
        return 63;
    else if (strcmp(name, "Knight attack") == 0)
        return 64;
    else if (strcmp(name, "Bishop xray attack") == 0)
        return 65;
    else if (strcmp(name, "Pinned") == 0)
        return 66;
    else if (strcmp(name, "Mobility") == 0)
        return 67;
    else if (strcmp(name, "Attack") == 0)
        return 68;
    else if (strcmp(name, "Minor threat") == 0)
        return 69;
    else if (strcmp(name, "Check") == 0)
        return 70;
    else if (strcmp(name, "King attackers count") == 0)
        return 71;
    else if (strcmp(name, "King attacks") == 0)
        return 72;
    else if (strcmp(name, "Knight on queen") == 0)
        return 73;
    else if (strcmp(name, "Knight defender") == 0)
        return 74;
    else if (strcmp(name, "Pinned direction") == 0)
        return 75;
    else if (strcmp(name, "Rook xray attack") == 0)
        return 76;
    else if (strcmp(name, "Queen attack") == 0)
        return 77;
    else if (strcmp(name, "Queen attack diagonal") == 0)
        return 78;
    else if (strcmp(name, "Blockers for king") == 0)
        return 79;
    else if (strcmp(name, "Slider on queen") == 0)
        return 80;
    else if (strcmp(name, "Pawn attack") == 0)
        return 81;
    else if (strcmp(name, "Threat safe pawn") == 0)
        return 82;
    else if (strcmp(name, "Restricted") == 0)
        return 83;
    else if (strcmp(name, "King ring") == 0)
        return 84;
    else if (strcmp(name, "King attackers weight") == 0)
        return 85;
    else if (strcmp(name, "King danger") == 0)
        return 86;
    else if (strcmp(name, "Weak bonus") == 0)
        return 87;
    else if (strcmp(name, "Rook threat") == 0)
        return 88;
    else if (strcmp(name, "Safe check") == 0)
        return 89;
    else if (strcmp(name, "Weak squares") == 0)
        return 90;
    else if (strcmp(name, "Weak queen protection") == 0)
        return 91;
    else if (strcmp(name, "Mobility bonus") == 0)
        return 92;
    else if (strcmp(name, "King distance") == 0)
        return 93;
    else if (strcmp(name, "Mobility eg") == 0)
        return 94;
    else if (strcmp(name, "Hanging") == 0)
        return 95;
    else if (strcmp(name, "King threat") == 0)
        return 96;
    else if (strcmp(name, "Pawn push threat") == 0)
        return 97;
    else if (strcmp(name, "Weak enemies") == 0)
        return 98;
    else if (strcmp(name, "Threats eg") == 0)
        return 99;
    else if (strcmp(name, "King attack") == 0)
        return 100; // Duplicate, should be 100 as per list
    else if (strcmp(name, "Safe pawn") == 0)
        return 101;
    else if (strcmp(name, "Flank attack") == 0)
        return 102;
    else if (strcmp(name, "Flank defense") == 0)
        return 103;
    else if (strcmp(name, "Passed leverable") == 0)
        return 104;
    else if (strcmp(name, "Queen count") == 0)
        return 105;
    else if (strcmp(name, "*Knight attack") == 0)
        return 106; // Note the asterisk here
    else if (strcmp(name, "Passed file") == 0)
        return 107;
    else if (strcmp(name, "Passed eg") == 0)
        return 108;
    else if (strcmp(name, "Winnable") == 0)
        return 109;
    else if (strcmp(name, "File") == 0)
        return 110;
    else if (strcmp(name, "Shelter strength") == 0)
        return 111;
    else if (strcmp(name, "Shelter storm") == 0)
        return 112;
    else if (strcmp(name, "Pawnless flank") == 0)
        return 113;
    else if (strcmp(name, "Unsafe checks") == 0)
        return 114;
    else if (strcmp(name, "King eg") == 0)
        return 115;
    else if (strcmp(name, "Strength square") == 0)
        return 116;
    else if (strcmp(name, "Storm square") == 0)
        return 117;
    else if (strcmp(name, "Endgame shelter") == 0)
        return 118;
    else if (strcmp(name, "Winnable total eg") == 0)
        return 119;
    else if (strcmp(name, "Weak lever") == 0)
        return 120;
    else if (strcmp(name, "King pawn distance") == 0)
        return 121;
    else if (strcmp(name, "Pawn count") == 0)
        return 122;
    else if (strcmp(name, "Knight count") == 0)
        return 123;
    else if (strcmp(name, "Piece count") == 0)
        return 124;
    return -1;
}

void dfs_topological_sort(GraphNode *node)
{
    if (node->visited == 2)
    {
        return;
    }
    if (node->visited == 1)
    {
        return;
    }

    node->visited = 1;

    for (int i = 0; i < node->calls_count; i++)
    {
        dfs_topological_sort(node->calls[i]);
    }

    node->visited = 2;
    printf("%s\n", node->name);
}

void free_graph()
{
    for (int i = 0; i < node_count; i++)
    {
        free(all_nodes[i]->name);
        free(all_nodes[i]->calls);
        free(all_nodes[i]->called_by);
        free(all_nodes[i]);
    }
    free(all_nodes);
}

void displayGraph()
{
    printf("\n--- All Graph Nodes (%d total) ---\n", node_count);
    for (int i = 0; i < node_count; ++i)
    {
        if (all_nodes[i] != NULL)
        {
            printf("%d %s\n", i, all_nodes[i]->name);
        }
        else
        {
            printf("%d (NULL Node Pointer)\n", i); // Should not happen if memory management is correct
        }
    }
    printf("----------------------------------\n");
}

void reset_graph()
{
    for (int i = 0; i < node_count; i++)
    {
        all_nodes[i]->visited = 0;
    }
}

int initialize_graph_from_file()
{
    const char *filename = "graph.txt";

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening graph file (graph.txt)");
        return -1;
    }

    char line[1024];
    GraphNode *current_node = NULL;

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "*", 1) == 0 && strcmp(line, "*end") != 0)
        {
            char *node_name = line + 1;
            trim_whitespace(node_name);
            current_node = find_or_create_node(node_name);
        }
        else if (strstr(line, "calls:") == line)
        {
            if (current_node)
            {
                char *connections_str = line + strlen("calls:");
                parse_connections(current_node, connections_str, 1);
            }
        }
        else if (strstr(line, "called by:") == line)
        {
            if (current_node)
            {
                char *connections_str = line + strlen("called by:");
                parse_connections(current_node, connections_str, 0);
            }
        }
    }

    fclose(file);
    return 0;
}

int sumNode(const ChessPosition *pos,
            const ChessPosition *pos2,
            int (*func)(ChessPosition *, ChessPosition *, Square *, void *, bool *),
            void *param, GraphNode *node, bool colorflipped)
{
    int total = 0;
    int total_flipped = 0;
    Square square;
    for (square.x = 0; square.x < 8; square.x++)
    {
        for (square.y = 0; square.y < 8; square.y++)
        {

            node->value[square.x][square.y] = func((ChessPosition *)pos, (ChessPosition *)pos2, &square, param, false);
            total += node->value[square.x][square.y];

            node->colorflip_value[square.x][square.y] = func((ChessPosition *)pos, (ChessPosition *)pos2, &square, param, true);
            total_flipped += node->colorflip_value[square.x][square.y];
        }
    }

    node->total_value = total;
    node->colorflip_total_value = total_flipped;

    return colorflipped ? total_flipped : total;
}

// int dummy_piece_value_mg(ChessPosition *pos, ChessPosition *pos2, Square *square, void *param, bool colorflipped) {}

int main_function_handler(const char *name, ChessPosition *pos, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    // example usage of param
    // main_function_handler(name,pos,pos2,square,&(bool){true}, false);
    // bool boolean = *(bool *)param;
    int node_idx = get_function_index(name);

    GraphNode *node = NULL;
    if (node_idx >= 0 && node_idx < node_count)
    {
        node = all_nodes[node_idx];
    }
    if (node == NULL)
    {
        fprintf(stderr, "Error: GraphNode '%s' not found or invalid index (%d). Cannot evaluate.\n", name, node_idx);
        return 0;
    }

    int result = 0;

    if (node->visited == 0)
    {
        node->visited = 1;
        if (strcmp(name, "Main evaluation") == 0)
        {
            result = main_evaluation(pos);
            node->total_value = result;
        }
        else if (strcmp(name, "Middle game evaluation") == 0)
        {
            result = dummy_middle_game_evaluation(pos, pos2, colorflipped);
            node->total_value = result;
        }
        else if (strcmp(name, "End game evaluation") == 0)
        {
            result = dummy_end_game_evaluation(pos, pos2, colorflipped);
            node->total_value = result;
        }
        else if (strcmp(name, "Piece value mg") == 0)
        {
            result = sumNode(pos, pos2, dummy_piece_value_mg, NULL, node, colorflipped);
        }
    }

    int *cache_total_value;
    int (*cache_value_matrix)[8];

    if (colorflipped)
    {
        cache_total_value = &node->colorflip_total_value;
        cache_value_matrix = node->colorflip_value;
    }
    else
    {
        cache_total_value = &node->total_value;
        cache_value_matrix = node->value;
    }

    if (square == NULL)
    {
        return *cache_total_value;
    }
    else
    {
        if (square->x >= 0 && square->x < 8 && square->y >= 0 && square->y < 8)
        {
            return cache_value_matrix[square->x][square->y];
        }
        else
        {
            fprintf(stderr, "Warning: Invalid square coordinates for cached lookup of '%s'.\n", name);
            return 0;
        }
    }
    return result;
}