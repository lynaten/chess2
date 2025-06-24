#include "header/tree/treeSearch.h"

int g_nodesExplored = 0;
int g_maxDepthReached = 0;
/* tree_search.c ---------------------------------------------------------- */
static inline bool isEngineSide(const ChessPosition *n,
                                bool engineIsWhite) /* root->w           */
{
    return n->w == engineIsWhite;
}

/* depth-first, back-track when opponent would lose ≥500                   */
static void dfsPruned(NodeAddress cur,
                      int depth,
                      bool engineIsWhite,
                      ChessTree root,
                      bool debug)
{
    if (!cur)
        return;

    g_nodesExplored++;
    if (depth > g_maxDepthReached)
        g_maxDepthReached = depth;

    if (depth == 0)
        return;

    int totalMoves;
    Move *moves = combination(&cur->info, &totalMoves);

    for (int i = 0; i < totalMoves; ++i)
    {
        ChessPosition nxt = cur->info;
        move(moves[i], &nxt);
        nxt.lastMove = moves[i];
        nxt.evaluationBar = main_evaluation(&nxt);

        int delta = nxt.evaluationBar - cur->info.evaluationBar;
        bool sideJustMovedIsEnemy = !isEngineSide(&cur->info, engineIsWhite);
        bool sideJustMovedIsUs = !sideJustMovedIsEnemy;

        // Prune if the side that just moved made a big blunder or allowed a huge gain
        if ((sideJustMovedIsUs &&
             ((engineIsWhite && delta <= -EVAL_DROP_THRESHOLD) ||
              (!engineIsWhite && delta >= EVAL_DROP_THRESHOLD))) ||
            (sideJustMovedIsEnemy &&
             ((engineIsWhite && delta >= EVAL_DROP_THRESHOLD) ||
              (!engineIsWhite && delta <= -EVAL_DROP_THRESHOLD))))
        {
            continue;
        }

        NodeAddress child = createTreeNode(&nxt);
        addChild(cur, child);

        if (debug)
        {
            Move m = nxt.lastMove;
            printf("[DFS] Added child: %c%d → %c%d | Eval: %d | Depth: %d | Total Nodes: %d\n",
                   'a' + m.before.x, 8 - m.before.y,
                   'a' + m.after.x, 8 - m.after.y,
                   nxt.evaluationBar,
                   depth,
                   g_nodesExplored);
        }

        dfsPruned(child, depth - 1, engineIsWhite, root, debug);
    }

    free(moves);
}

/* breadth-first with a priority queue (best score first) -----------------*/
typedef struct
{
    NodeAddress n;
    int depth;
} QEntry;

static int compareEval(const QEntry *a, const QEntry *b, bool engineIsWhite)
{
    if (engineIsWhite)
        return b->n->info.evaluationBar - a->n->info.evaluationBar; // max-heap
    else
        return a->n->info.evaluationBar - b->n->info.evaluationBar; // min-heap
}

typedef struct
{
    QEntry data[MAX_BB_NODES];
    int size;
    bool engineIsWhite;
} PriorityQueue;

static void pq_init(PriorityQueue *pq, bool engineIsWhite)
{
    pq->size = 0;
    pq->engineIsWhite = engineIsWhite;
}

static void pq_push(PriorityQueue *pq, QEntry item)
{
    int i = pq->size++;
    pq->data[i] = item;

    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (compareEval(&pq->data[i], &pq->data[parent], pq->engineIsWhite) <= 0)
            break;
        QEntry tmp = pq->data[i];
        pq->data[i] = pq->data[parent];
        pq->data[parent] = tmp;
        i = parent;
    }
}

static QEntry pq_pop(PriorityQueue *pq)
{
    QEntry top = pq->data[0];
    pq->data[0] = pq->data[--pq->size];

    int i = 0;
    while (1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int best = i;

        if (left < pq->size &&
            compareEval(&pq->data[left], &pq->data[best], pq->engineIsWhite) > 0)
            best = left;
        if (right < pq->size &&
            compareEval(&pq->data[right], &pq->data[best], pq->engineIsWhite) > 0)
            best = right;

        if (best == i)
            break;

        QEntry tmp = pq->data[i];
        pq->data[i] = pq->data[best];
        pq->data[best] = tmp;
        i = best;
    }
    return top;
}

static bool pq_empty(const PriorityQueue *pq)
{
    return pq->size == 0;
}

static void bbBfs(NodeAddress root,
                  int depthLimit,
                  int nodeCap,
                  bool debug)
{
    bool engineIsWhite = root->info.w;
    PriorityQueue pq;
    pq_init(&pq, engineIsWhite);

    pq_push(&pq, (QEntry){root, 0});
    int nodesSeen = 1;
    g_nodesExplored = 1;
    g_maxDepthReached = 0;

    while (!pq_empty(&pq))
    {
        QEntry cur = pq_pop(&pq);
        if (cur.depth > g_maxDepthReached)
            g_maxDepthReached = cur.depth;
        if (cur.depth == depthLimit)
            continue;

        int totalMoves;
        Move *moves = combination(&cur.n->info, &totalMoves);

        for (int i = 0; i < totalMoves; ++i)
        {
            g_nodesExplored++;
            ChessPosition nxt = cur.n->info;
            move(moves[i], &nxt);
            nxt.lastMove = moves[i];
            nxt.evaluationBar = main_evaluation(&nxt);

            int delta = nxt.evaluationBar - cur.n->info.evaluationBar;
            bool sideJustMovedIsEnemy = !isEngineSide(&cur.n->info, engineIsWhite);
            bool sideJustMovedIsUs = !sideJustMovedIsEnemy;

            // Check for blunder
            if ((sideJustMovedIsUs &&
                 ((engineIsWhite && delta <= -EVAL_DROP_THRESHOLD) ||
                  (!engineIsWhite && delta >= EVAL_DROP_THRESHOLD))) ||
                (sideJustMovedIsEnemy &&
                 ((engineIsWhite && delta >= EVAL_DROP_THRESHOLD) ||
                  (!engineIsWhite && delta <= -EVAL_DROP_THRESHOLD))))
            {
                if (abs(delta) >= 500)
                {
                    // If a severe blunder, this move and its resulting branch are simply ignored.
                    // The parent node is NOT deleted.
                    continue; // Skip this child and its subtree
                }
                continue; // Skip this child
            }

            NodeAddress child = createTreeNode(&nxt);
            addChild(cur.n, child);

            if (debug)
            {
                Move m = nxt.lastMove;
                printf("[BFS] Added child: %c%d → %c%d | Eval: %d | Depth: %d | Total Nodes: %d\n",
                       'a' + m.before.x, 8 - m.before.y,
                       'a' + m.after.x, 8 - m.after.y,
                       nxt.evaluationBar,
                       cur.depth + 1,
                       g_nodesExplored);
            }

            if (nodeCap && ++nodesSeen >= nodeCap)
            {
                free(moves);
                return;
            }

            pq_push(&pq, (QEntry){child, cur.depth + 1});
        }

        free(moves);
        // The 'shouldDeleteParent' logic and deleteTree call are removed entirely.
    }
}

ChessTree buildTree(const ChessPosition *rootPos,
                    int depth,
                    SearchStyle style,
                    int nodeLimit,
                    bool debug)
{
    ChessTree root = createTreeNode(rootPos);

    switch (style)
    {
    case SEARCH_DFS_PRUNE:
        dfsPruned(root, depth, rootPos->w, root, debug);
        break;

    case SEARCH_BB_BFS:
        bbBfs(root, depth, nodeLimit ? nodeLimit : MAX_BB_NODES, debug);
        break;
    }
    return root;
}

int leafSum(NodeAddress n)
{
    if (!n->children)
        return n->info.evaluationBar;
    int s = 0;
    for (ChildAddress c = n->children; c; c = c->next)
        s += leafSum(c->info);
    return s;
}

Move chooseMoveByLeafSum(ChessTree root, bool maximize)
{
    Move best = {.before = {-1, -1}, .after = {-1, -1}};
    long long bestScore = maximize ? LLONG_MIN : LLONG_MAX;

    for (ChildAddress c = root->children; c; c = c->next)
    {
        long long score = leafSum(c->info);
        bool isBetter = maximize ? (score > bestScore) : (score < bestScore);
        if (isBetter)
        {
            bestScore = score;
            best = c->info->info.lastMove;
        }
    }

    printf("Leaf-sum %s = %lld\n", maximize ? "max" : "min", bestScore);
    return best;
}