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

    for (int y = 0; y < 8; y++) // Iterate rows (y) first for board access [y][x]
    {
        for (int x = 0; x < 8; x++)
        {
            if (board(pos, x, y) == kingChar) // Using board(pos, x, y)
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
        // This could indicate an error in board setup or a king capture.
        return true;
    }

    Square kingSquare = {kingX, kingY};

    // Use king_moves to find all legal king escapes
    int moveCount = 0;
    // Assuming king_moves function correctly generates pseudo-legal king moves
    // and that the second Square* parameter (targetSquare) can be NULL if not needed.
    Move *moves = king_moves(pos, &kingSquare, NULL, &moveCount); // Corrected: targetSquare can be NULL or specific

    // IMPORTANT: After generating king moves, you should verify if they are *legal*
    // meaning they don't move the king into check. Your current unsafe_checks call
    // only checks if the king *is* in check, not if a potential move leads to check.
    // This is a common oversight in basic checkmate logic.
    // For a robust checkmate, you typically need to:
    // 1. Check if the current player's king is *currently* in check.
    // 2. Generate ALL pseudo-legal moves for the current player.
    // 3. For each pseudo-legal move, make it, then check if their king is *still* in check.
    //    If the king is not in check after the move, it's a legal move and not checkmate.
    // If no such legal moves exist AND the king is currently in check, it's checkmate.
    // If no such legal moves exist BUT the king is NOT currently in check, it's stalemate.

    // Your current logic:
    // If king_moves returns any moves, it assumes they are escapes. This is flawed.
    // A king move might move the king from one checked square to another checked square.
    // To correctly check for checkmate/stalemate, you need a full legal move generator
    // that filters out moves that leave the king in check.

    // Placeholder for robust checkmate logic:
    // This is a simplified check that assumes king_moves only returns valid escape squares.
    // For production, you'd need a more comprehensive legal move generator.
    if (moveCount > 0)
    {
        // If the 'king_moves' function is designed to return *only* legal, non-checked escape moves,
        // then this logic holds. Otherwise, it's a simplification.
        bool hasLegalKingEscape = false;
        for (int i = 0; i < moveCount; i++) {
            ChessPosition tempPos = *pos; // Create a temporary copy
            move(moves[i], &tempPos); // Apply the potential move
            // After the move, check if the king (now at moves[i].after) is safe
            if (!unsafe_checks(&tempPos, &(moves[i].after), NULL)) {
                hasLegalKingEscape = true;
                break;
            }
        }
        free(moves);
        if (hasLegalKingEscape) {
            return false; // Found at least one legal escape
        }
    }
    else
    {
        free(moves); // free if moveCount was 0 initially
    }


    // No legal king moves (either initially or after filtering for check)
    // Now, determine if it's checkmate or stalemate
    if (unsafe_checks(pos, &kingSquare, NULL)) // Check if king is currently under attack
    {
        // In check and no legal moves => checkmate
        return true;
    }
    else
    {
        // Not in check, but no legal moves => stalemate
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

int fileCharToInt(char f) {
    return tolower(f) - 'a';
}

int rankCharToInt(char r) {
    return 8 - (r - '0');
}

bool getUserCoordinates(ChessPosition *pos, Move best_move, Move *userMove)
{
    // Note: displayBoard and current position evaluation prompt are now assumed
    // to be handled in the gameLoop *before* calling this function.

    displayBoard(*pos);
    printf("Eval points: %d", pos->evaluationBar);
    char beforeX_char[2], afterX_char[2]; // e.g., "a"
    int beforeY_int, afterY_int;         // e.g., 2 (for rank 2)

    printf("Enter your move [Suggested: %c%d %c%d]: ",
           'a' + best_move.before.x, 8 - best_move.before.y,
           'a' + best_move.after.x, 8 - best_move.after.y);
    fflush(stdout); // Ensure prompt is displayed before scanf

    // Loop until valid input format
    while (scanf("%1s %d %1s %d", beforeX_char, &beforeY_int, afterX_char, &afterY_int) != 4)
    {
        // Clear invalid input from buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Invalid input format. Please use 'e.g., a2 a4'. Try again.\n");
        printf("Enter your move [Suggested: %c%d %c%d]: ",
               'a' + best_move.before.x, 8 - best_move.before.y,
               'a' + best_move.after.x, 8 - best_move.after.y);
        fflush(stdout);
    }

    // Convert input to internal coordinates (0-7)
    userMove->before.x = fileCharToInt(beforeX_char[0]);
    userMove->before.y = rankCharToInt(beforeY_int + '0'); // Convert int to char then to int coord
    userMove->after.x = fileCharToInt(afterX_char[0]);
    userMove->after.y = rankCharToInt(afterY_int + '0');

    // --- Validate basic coordinates ---
    if (userMove->before.x < 0 || userMove->before.x > 7 ||
        userMove->before.y < 0 || userMove->before.y > 7 ||
        userMove->after.x < 0 || userMove->after.x > 7 ||
        userMove->after.y < 0 || userMove->after.y > 7)
    {
        printf("Invalid coordinates entered. Coordinates must be within a1-h8.\n");
        return false; // Invalid coordinate range
    }

    return true; // Successfully obtained coordinates within range
}

bool populateMoveDetails(ChessPosition *pos, Move *userMove)
{
    // Initialize default values for the flags and promotion_piece
    userMove->is_castling = false;
    userMove->is_en_passant = false;
    userMove->promotion_piece = '\0'; // Null character for no promotion
    userMove->captured_piece = '\0';   // Default to no captured piece

    // Get the piece being moved from the board at the 'before' square
    userMove->piece_moved = pos->b[userMove->before.x][userMove->before.y]; // Corrected access [x][y]

    // Determine captured_piece *if* the destination square is not empty
    if (pos->b[userMove->after.x][userMove->after.y] != '-') { // Corrected access [x][y]
        userMove->captured_piece = pos->b[userMove->after.x][userMove->after.y]; // Corrected access [x][y]
    }

    // Check for Castling (King moves two squares horizontally)
    char moved_piece_upper = toupper(userMove->piece_moved);
    if (moved_piece_upper == 'K') {
        int dx = userMove->after.x - userMove->before.x;
        if (dx == 2 || dx == -2) { // King moved two squares horizontally
            userMove->is_castling = true;
        }
    }

    // Check for En Passant
    if (moved_piece_upper == 'P') { // If a pawn moved
        // An en passant capture is a diagonal pawn move to an empty square,
        // specifically targeting the en passant square.
        if (userMove->after.x == pos->e[0] && userMove->after.y == pos->e[1] && userMove->captured_piece == '-') {
            // Confirm it's actually a diagonal pawn move (necessary because captured_piece == '-' is also true for non-capture pawn advances)
            if (abs(userMove->after.x - userMove->before.x) == 1 &&
                abs(userMove->after.y - userMove->before.y) == 1)
            {
                userMove->is_en_passant = true;
                // The captured piece in en passant is the pawn on the correct adjacent rank
                if (pos->w) { // White to move (captures black pawn)
                    userMove->captured_piece = 'p';
                } else { // Black to move (captures white pawn)
                    userMove->captured_piece = 'P';
                }
            }
        }
    }

    // Check for Promotion
    if (moved_piece_upper == 'P') { // If a pawn moved
        // Check if pawn reached the last rank (rank 8 for white, rank 1 for black)
        if ((pos->w && userMove->after.y == 0) || (!pos->w && userMove->after.y == 7)) {
            char promo_char[2];
            printf("Pawn promotion! Promote to (Q, R, B, N): ");
            fflush(stdout);
            // Loop until valid promotion piece is entered
            while (scanf("%1s", promo_char) != 1 || !strchr("QRBNqrbn", promo_char[0])) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF); // Clear buffer
                printf("Invalid promotion piece. Enter (Q, R, B, N): ");
                fflush(stdout);
            }
            if (pos->w) { // White to move
                userMove->promotion_piece = toupper(promo_char[0]);
            } else { // Black to move
                userMove->promotion_piece = tolower(promo_char[0]);
            }
        }
    }

    return true; // Successfully populated all details (or handled promotion input)
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
        Move best = {.before = {-1, -1}, .after = {-1, -1}, .piece_moved = '\0', .captured_piece = '\0', .promotion_piece = '\0', .is_castling = false, .is_en_passant = false};

        
        if (!useEngine) {
            if (pos->w) {
                best.before.x = 4; best.before.y = 6;
                best.after.x = 4; best.after.y = 4;
            } else {
                best.before.x = 4; best.before.y = 1;
                best.after.x = 4; best.after.y = 3;
            }
            best.piece_moved = pos->b[best.before.x][best.before.y];
        }

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

    
        Move inputMove;

        while (true){
            if (getUserCoordinates(pos, best, &inputMove))
            {
                populateMoveDetails(pos, &inputMove);
                if (isMoveInList(combi, totalMoves, inputMove))
                {
                    printf("Move is valid! Making the move.\n");
                    move(inputMove, pos);
                    pos->evaluationBar = main_evaluation(pos);
                    break;

                }
                else
                {
                    printf("Illegal move. Try again.\n");
                }
            }
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

    clock_t evalStart = clock();
    pos.evaluationBar = main_evaluation(&pos);
    clock_t evalEnd = clock();

    double elapsed_ms = 1000.0 * (evalEnd - evalStart) / CLOCKS_PER_SEC;
    printf("[Eval Timing] Evaluation took %.3f ms\n", elapsed_ms);
    gameLoop(&pos);
    return 0;
}

// watch -n 0.5 grep VmRSS /proc/$(pidof chess_sim)/status
// gdb ./bin/chess_sim
// make clean build run