/* treeSearch.h ---------------------------------------------------------- */
#ifndef TREESEARCH_H
#define TREESEARCH_H

#include "tree.h"

typedef enum
{
    SEARCH_DFS_PRUNE, /* depth-first with opponent pruning        */
    SEARCH_BB_BFS     /* branch-&-bound breadth-first, PQ + limit */
} SearchStyle;

/* User-tunable knobs */
#define EVAL_DROP_THRESHOLD 10 /* enemy will never blunder ≥500       */
#define MAX_BB_NODES 2500      /* hard ceiling for branch-&-bound     */

/* Build a tree that obeys the chosen style.  Depth is *still* respected.  */
ChessTree buildTree(const ChessPosition *rootPos,
                    int depth,
                    SearchStyle style,
                    int nodeLimit,
                    bool debug); /* 0 ⇒ unlimited     */

Move chooseMoveByLeafSum(ChessTree root, bool maximize);

int leafSum(NodeAddress n);

extern int g_nodesExplored;
extern int g_maxDepthReached;

#endif /* TREE_SEARCH_H */
