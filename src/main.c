#include <stdio.h>
#include <stdbool.h>
#include <ctype.h> // for tolower()
#include "header/chess/chess.h"
#include "header/chess/evaluation.h"
#include "header/chess/combination.h"
#include "header/tree/treeSearch.h"
#include <time.h> // for clock()
#include <limits.h>

static bool isCheckmate(ChessPosition *pos)
{
    // Find current player's king position
    char kingChar = (pos->w ? 'K' : 'k');
    int kingX = -1, kingY = -1;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == kingChar)
            {
                kingX = x;
                kingY = y;
                break;
            }
        }
        if (kingX != -1)
            break;
    }

    if (kingX == -1)
    {
        // King not found — invalid state, treat as checkmate
        return true;
    }

    Square kingSquare = {kingX, kingY};

    // Use king_moves to find all legal king escapes
    int moveCount = 0;
    Move *moves = king_moves(pos, &kingSquare, &kingSquare, &moveCount);

    if (moveCount > 0)
    {
        // At least one escape move for king — not checkmate
        free(moves);
        return false;
    }

    free(moves);

    // No legal king moves — now check if king is under attack
    if (unsafe_checks(pos, &kingSquare, NULL))
    {
        // In check and no escape => checkmate
        return true;
    }
    else
    {
        // Not in check, no legal moves => stalemate
        return false;
    }
}

bool isMoveInList(Move *combi, int totalMoves, Move move)
{
    for (int i = 0; i < totalMoves; i++)
    {
        if (combi[i].before.x == move.before.x &&
            combi[i].before.y == move.before.y &&
            combi[i].after.x == move.after.x &&
            combi[i].after.y == move.after.y)
        {
            return true;
        }
    }
    return false;
}

void gameLoop(ChessPosition *pos)
{
    while (true)
    {
        char eng;
        bool useEngine = false;
        printf("Let engine play this turn? (y/n): ");
        scanf(" %c", &eng);
        useEngine = (eng == 'y' || eng == 'Y');

        int totalMoves;
        Move *combi = combination(pos, &totalMoves);
        Move best = {.before = {-1, -1}, .after = {-1, -1}};

        if (useEngine)
        {
            char dbg;
            bool debug = false;
            printf("Enable debug this turn? (y/n): ");
            scanf(" %c", &dbg);
            debug = (dbg == 'y' || dbg == 'Y');

            printf("Engine is thinking...\n");

            int depth = 3;
            SearchStyle st = SEARCH_DFS_PRUNE;
            int nodeCap = 0;

            g_nodesExplored = 0;
            g_maxDepthReached = 0;

            clock_t start = clock();

            ChessTree t = buildTree(pos, depth, st, nodeCap, debug);
            printf("Top-level move evaluations (leaf sum):\n");
            for (ChildAddress c = t->children; c; c = c->next)
            {
                Move m = c->info->info.lastMove;
                int leafSumResult = leafSum(c->info);
                printf("Move %c%d → %c%d → Leaf-Sum = %d\n",
                       'a' + m.before.x, 8 - m.before.y,
                       'a' + m.after.x, 8 - m.after.y,
                       leafSumResult);
            }
            best = chooseMoveByLeafSum(t, pos->w);

            clock_t end = clock();
            double elapsedMs = 1000.0 * (end - start) / CLOCKS_PER_SEC;

            printf("[Debug] Nodes explored: %d\n", g_nodesExplored);
            printf("[Debug] Max depth reached: %d\n", g_maxDepthReached);
            printf("[Debug] Search time: %.2f ms\n", elapsedMs);

            freeTree(t);
            printf("[Debug] Freed nodes: %d\n", g_nodesFreed);
            free(combi);
        }

        char beforeX[2];
        int beforeY;
        char afterX[2];
        int afterY;

        if (isCheckmate(pos))
        {
            displayBoard(*pos);
            if (pos->w)
            {
                printf("Checkmate! White to move => Black has delivered mate.\n");
                printf("Black wins!\n");
            }
            else
            {
                printf("Checkmate! Black to move => White has delivered mate.\n");
                printf("White wins!\n");
            }
            break;
        }

        displayBoard(*pos);
        printf("current position evaluation: %d\n", pos->evaluationBar);

        printf("Enter your move [Suggested: %c%d → %c%d]: ",
               'a' + best.before.x, 8 - best.before.y,
               'a' + best.after.x, 8 - best.after.y);
        fflush(stdout);

        if (scanf("%1s %d %1s %d", beforeX, &beforeY, afterX, &afterY) != 4)
        {
            while (getchar() != '\n')
            {
            }
            printf("Invalid input format. Try again.\n");
            continue;
        }

        Move inputMove;
        inputMove.before.x = alphabetToIndex(beforeX[0]);
        inputMove.before.y = 8 - beforeY;
        inputMove.after.x = alphabetToIndex(afterX[0]);
        inputMove.after.y = 8 - afterY;

        if (isMoveInList(combi, totalMoves, inputMove))
        {
            printf("Move is valid! Making the move.\n");
            move(inputMove, pos);
            pos->evaluationBar = main_evaluation(pos);
        }
        else
        {
            printf("Illegal move. Try again.\n");
        }
    }
}

int main(void)
{
    ChessPosition pos = {
        // b
        .b = {
            {'r', 'p', '-', '-', '-', '-', 'P', 'R'},
            {'n', 'p', '-', '-', '-', '-', 'P', 'N'},
            {'b', 'p', '-', '-', '-', '-', 'P', 'B'},
            {'q', 'p', '-', '-', '-', '-', 'P', 'Q'},
            {'k', 'p', '-', '-', '-', '-', 'P', 'K'},
            {'b', 'p', '-', '-', '-', '-', 'P', 'B'},
            {'n', 'p', '-', '-', '-', '-', 'P', 'N'},
            {'r', 'p', '-', '-', '-', '-', 'P', 'R'}},
        .c = {true, true, true, true},
        .e = {-1, -1},
        .w = true,
        .m = {0, 1}};

    // clock_t evalStart = clock();
    pos.evaluationBar = main_evaluation(&pos);
    // clock_t evalEnd = clock();

    // double elapsed_ms = 1000.0 * (evalEnd - evalStart) / CLOCKS_PER_SEC;
    // printf("[Eval Timing] Evaluation took %.3f ms\n", elapsed_ms);
    gameLoop(&pos);
    return 0;
}

// watch -n 0.5 grep VmRSS /proc/$(pidof chess_sim)/status
// gdb ./bin/chess_sim
// make clean build run