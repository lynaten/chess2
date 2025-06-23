#ifndef TREE_H
#define TREE_H

#include "header/chess/chess.h"
#include "header/chess/evaluation.h"
#include "header/chess/combination.h"
/* -------------------------------------------------------------------------
   TREE ADT
   ------------------------------------------------------------------------- */

typedef struct chessTreeNode *NodeAddress;
typedef struct childNode *ChildAddress;

extern int g_nodesFreed;

typedef struct chessTreeNode
{
    ChessPosition info;    // The current position (with evaluationBar, etc.)
    ChildAddress children; // Linked list of child nodes
    bool deleted;
} ChessTreeNode;

typedef struct childNode
{
    NodeAddress info;  // Pointer to the child tree node
    ChildAddress next; // Next sibling
} ChildNode;

typedef NodeAddress ChessTree;

NodeAddress createTreeNode(const ChessPosition *pos);
void addChild(NodeAddress parent, NodeAddress child);
void freeTree(ChessTree root);
void generateTree(NodeAddress root, int depth);
ChessTree initializeTree(const ChessPosition *initialPos, int depth);
void findBestPath(NodeAddress node, int depth, Move *bestPath, int *bestEvaluation, Move *currentPath, int currentDepth, int currentEvaluation);
void findBestEvaluationPath(ChessTree tree, int depth);
void deleteTree(NodeAddress n);
#endif