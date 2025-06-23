#include "header/tree/tree.h"
/* -------------------------------------------------------------------------
   TREE ADT IMPLEMENTATION
   ------------------------------------------------------------------------- */

/**
 * Create a new tree node, copying the ChessPosition data.
 */
NodeAddress createTreeNode(const ChessPosition *pos)
{
    NodeAddress node = (NodeAddress)malloc(sizeof(ChessTreeNode));
    if (!node)
    {
        Move m = pos->lastMove;
        fprintf(stderr, "[Malloc Error] Failed to allocate ChessTreeNode\n");

        if (m.before.x >= 0 && m.after.x >= 0)
        {
            fprintf(stderr, "  At move: %c%d → %c%d | Eval: %d\n",
                    'a' + m.before.x, 8 - m.before.y,
                    'a' + m.after.x, 8 - m.after.y,
                    pos->evaluationBar);
        }
        else
        {
            fprintf(stderr, "  At root position | Eval: %d\n", pos->evaluationBar);
        }

        exit(1);
    }

    node->info = *pos;
    node->children = NULL;
    node->deleted = false;
    return node;
}
/**
 * Adds a child to a parent's linked list of children.
 */
void addChild(NodeAddress parent, NodeAddress child)
{
    if (!parent || !child)
        return; // safeguard

    ChildAddress newChild = (ChildAddress)malloc(sizeof(ChildNode));
    if (!newChild)
    {
        Move m = child->info.lastMove;
        fprintf(stderr, "[Malloc Error] Failed to allocate ChildNode\n");

        if (m.before.x >= 0 && m.after.x >= 0)
        {
            fprintf(stderr, "  At move: %c%d → %c%d | Eval: %d\n",
                    'a' + m.before.x, 8 - m.before.y,
                    'a' + m.after.x, 8 - m.after.y,
                    child->info.evaluationBar);
        }
        else
        {
            fprintf(stderr, "  At root child node | Eval: %d\n", child->info.evaluationBar);
        }

        exit(1);
    }
    newChild->info = child;
    newChild->next = parent->children;
    parent->children = newChild;
}

void deleteTree(NodeAddress n)
{
    if (!n)
        return;

    n->deleted = true;
    // First recursively free all children
    ChildAddress child = n->children;
    while (child)
    {
        ChildAddress next = child->next;
        deleteTree(child->info); // Recursively delete child node
        free(child);             // Free the child link
        child = next;
    }

    // Then free this node itself
    free(n);
    g_nodesFreed++;
}

int g_nodesFreed = 0;

/**
 * Recursively frees a subtree.
 */
void freeTree(ChessTree root)
{
    if (!root || root->deleted)
        return;

    root->deleted = true;

    ChildAddress child = root->children;
    while (child)
    {
        ChildAddress temp = child->next;
        freeTree(child->info);
        free(child);
        child = temp;
    }

    free(root);
    g_nodesFreed++;
}

void generateTree(NodeAddress root, int depth)
{
    if (!root || depth <= 0)
        return;

    int totalMoves;
    Move *combi = combination(&(root->info), &totalMoves);

    // For each legal move, create a child node
    for (int i = 0; i < totalMoves; i++)
    {
        // Create a new chess position by applying the move
        ChessPosition newPos = root->info;
        move(combi[i], &newPos);

        // Set the last move in the new position
        newPos.lastMove = combi[i];

        // Calculate the evaluation bar for the new position
        newPos.evaluationBar = main_evaluation(&newPos);

        // Create a new tree node for the new position
        NodeAddress childNode = createTreeNode(&newPos);

        // Add the child node to the current node
        addChild(root, childNode);

        // Recursively generate the tree for the child node
        generateTree(childNode, depth - 1);
    }

    // Free the combination list
    free(combi);
}

/**
 * Initializes the chess tree for a given position and generates the tree up to the specified depth.
 */
ChessTree initializeTree(const ChessPosition *initialPos, int depth)
{
    // Create the root node with the initial position
    ChessTree tree = createTreeNode(initialPos);

    // Generate the tree up to the specified depth
    generateTree(tree, depth);

    return tree;
}

void findBestPath(NodeAddress node, int depth, Move *bestPath, int *bestEvaluation, Move *currentPath, int currentDepth, int currentEvaluation)
{
    if (!node)
        return;

    // Update evaluation with the current node's position evaluation
    currentEvaluation += node->info.evaluationBar;

    // If we've reached the max depth or there are no children, compare and update the best path
    if (currentDepth == depth || !node->children)
    {
        if (currentEvaluation > *bestEvaluation)
        {
            *bestEvaluation = currentEvaluation;
            // Copy the current path to the best path
            memcpy(bestPath, currentPath, currentDepth * sizeof(Move));
        }
        return;
    }

    // Traverse children nodes
    ChildAddress child = node->children;
    while (child)
    {
        // Use the stored lastMove from the child's info
        Move move = child->info->info.lastMove;

        // Add the move to the current path
        currentPath[currentDepth] = move;

        // Recursively find the best path
        findBestPath(child->info, depth, bestPath, bestEvaluation, currentPath, currentDepth + 1, currentEvaluation);

        child = child->next;
    }
}

static int minimax(NodeAddress node,
                   int depth,
                   bool maximizingSide, /* true = white to move */
                   Move *bestMoveOut)   /* best move from ‘node’ → child */
{
    /* -- leaf or depth limit -- */
    if (depth == 0 || node->children == NULL)
        return node->info.evaluationBar;

    int bestScore = maximizingSide ? INT_MIN : INT_MAX;
    Move bestMove = {.before = {-1, -1}, .after = {-1, -1}};

    for (ChildAddress ch = node->children; ch; ch = ch->next)
    {
        /* minimax for child; opposite side to move next */
        int childScore = minimax(ch->info,
                                 depth - 1,
                                 !maximizingSide,
                                 NULL);

        /* pick max or min */
        if ((maximizingSide && childScore > bestScore) ||
            (!maximizingSide && childScore < bestScore))
        {
            bestScore = childScore;
            bestMove = ch->info->info.lastMove;
        }
    }

    /* bubble the principal-variation move up */
    if (bestMoveOut)
        *bestMoveOut = bestMove;
    return bestScore;
}

/* Top-level helper that prints the principal variation of a given depth. */
void findBestEvaluationPath(ChessTree root, int depth)
{
    if (!root)
        return;

    /* allocate array large enough for the PV                           */
    Move pv[depth];
    NodeAddress cur = root;
    bool maximizingSide = root->info.w; /* white’s turn at root?  */

    /* run minimax once to get the best score and first move           */
    int bestScore = minimax(cur, depth, maximizingSide, &pv[0]);

    printf("Best line (depth %d)  score = %d\n", depth, bestScore);

    /* --- generate PV step-by-step --- */
    for (int ply = 0; ply < depth; ++ply)
    {
        Move m = pv[ply];
        if (m.before.x == -1)
            break; /* ran out of moves             */

        /* print the move in algebraic-like form                       */
        printf("Ply %2d: %c%d → %c%d\n",
               ply + 1,
               'a' + m.before.x, 8 - m.before.y,
               'a' + m.after.x, 8 - m.after.y);

        /* find the child that matches the move so we can continue     */
        ChildAddress c;
        for (c = cur->children; c; c = c->next)
        {
            if (memcmp(&c->info->info.lastMove, &m, sizeof(Move)) == 0)
                break;
        }
        if (!c)
            break; /* shouldn’t happen              */

        /* get next best move one ply deeper                           */
        cur = c->info;
        maximizingSide = !maximizingSide;
        if (ply + 1 < depth)
            minimax(cur, depth - (ply + 1), maximizingSide, &pv[ply + 1]);
    }
}