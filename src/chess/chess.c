#include "header/chess/chess.h"

/**
 * Retrieves a piece from the board if in range, otherwise returns 'x'.
 */
char board(const ChessPosition *pos, int x, int y)
{
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
        return pos->b[x][y];
    }
    return 'x';
}

/**
 * Flips the board “vertically” and toggles case (uppercase <-> lowercase),
 * flips castling rights, flips en passant, and toggles the side to move.
 *
 * Also copies evaluationBar, blackMoveCount, whiteMoveCount as-is or you
 * can decide if you want to manipulate them.
 */
void colorflip(const ChessPosition *pos, ChessPosition *flipped)
{
    // Flip the board vertically and toggle piece case
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            char piece = pos->b[x][7 - y]; // Flip vertically
            if (isupper(piece))
            {
                flipped->b[x][y] = tolower(piece); // Convert white to black
            }
            else if (islower(piece))
            {
                flipped->b[x][y] = toupper(piece); // Convert black to white
            }
            else
            {
                flipped->b[x][y] = piece; // Keep empty squares as is
            }
        }
    }

    // Flip castling rights
    flipped->c[0] = pos->c[2]; // White kingside -> Black kingside
    flipped->c[1] = pos->c[3]; // White queenside -> Black queenside
    flipped->c[2] = pos->c[0]; // Black kingside -> White kingside
    flipped->c[3] = pos->c[1]; // Black queenside -> White queenside

    // Flip en passant square (if it exists)
    if (pos->e[0] != -1)
    {
        flipped->e[0] = pos->e[0];
        flipped->e[1] = 7 - pos->e[1];
    }
    else
    {
        flipped->e[0] = -1;
        flipped->e[1] = -1;
    }

    // Flip turn
    flipped->w = !pos->w;

    // Copy move counters
    flipped->m[0] = pos->m[0];
    flipped->m[1] = pos->m[1];
}

/**
 * Summation over all squares using the provided callback function.
 */
int sum(const ChessPosition *pos,
        int (*func)(ChessPosition *, Square *, void *),
        void *param)
{
    int total = 0;
    Square square;
    for (square.x = 0; square.x < 8; square.x++)
    {
        for (square.y = 0; square.y < 8; square.y++)
        {
            total += func((ChessPosition *)pos, &square, param);
        }
    }
    return total;
}

void displayBoard(ChessPosition pos)
{
    if (pos.w) // White's perspective
    {
        printf("    a b c d e f g h    \n");
        printf("   -----------------   \n");
        for (int i = 0; i < 8; i++)
        {
            printf("%d | ", (8 - i));
            for (int j = 0; j < 8; j++)
            {
                printf("%c ", pos.b[j][i]); // Access board normally for White
            }
            printf("| %d\n", (8 - i));
        }
        printf("   -----------------   \n");
        printf("    a b c d e f g h    \n");
    }
    else // Black's perspective
    {
        printf("    h g f e d c b a    \n"); // Flipped file labels
        printf("   -----------------   \n");
        for (int i = 7; i >= 0; i--) // Reverse rows
        {
            printf("%d | ", (8 - i));
            for (int j = 7; j >= 0; j--) // Reverse columns
            {
                printf("%c ", pos.b[j][i]); // Access flipped board for Black
            }
            printf("| %d\n", (8 - i));
        }
        printf("   -----------------   \n");
        printf("    h g f e d c b a    \n"); // Flipped file labels
    }
}

int alphabetToIndex(char letter)
{
    return letter - 'a';
}

// Helper to update castling rights based on moves
static void update_castling_rights_after_move(ChessPosition *pos, Move m, char actual_captured_piece_on_board) {
    char moved_piece_upper = toupper(m.piece_moved);
    char captured_piece_upper = toupper(actual_captured_piece_on_board); // Use actual piece for capture checks

    // If a king moves, that side loses all castling rights
    if (moved_piece_upper == 'K') {
        if (m.piece_moved == 'K') { // White King
            pos->c[0] = false; // White King-side
            pos->c[1] = false; // White Queen-side
        } else { // Black King
            pos->c[2] = false; // Black King-side
            pos->c[3] = false; // Black Queen-side
        }
    }

    // If a rook moves from its original square, that side loses specific castling right
    if (moved_piece_upper == 'R') {
        if (m.piece_moved == 'R') { // White Rook
            if (m.before.x == 0 && m.before.y == 7) pos->c[1] = false; // White Q-side rook (a1)
            if (m.before.x == 7 && m.before.y == 7) pos->c[0] = false; // White K-side rook (h1)
        } else { // Black Rook
            if (m.before.x == 0 && m.before.y == 0) pos->c[3] = false; // Black Q-side rook (a8)
            if (m.before.x == 7 && m.before.y == 0) pos->c[2] = false; // Black K-side rook (h8)
        }
    }

    // If an enemy rook is captured on its original corner square, the *opponent* loses castling right
    // (We use 'actual_captured_piece_on_board' here to reflect what was truly captured)
    if (captured_piece_upper == 'R') {
        if (actual_captured_piece_on_board == 'r') { // Black Rook captured
            if (m.after.x == 0 && m.after.y == 0) pos->c[3] = false; // Black Q-side rook captured at a8
            if (m.after.x == 7 && m.after.y == 0) pos->c[2] = false; // Black K-side rook captured at h8
        } else if (actual_captured_piece_on_board == 'R') { // White Rook captured
            if (m.after.x == 0 && m.after.y == 7) pos->c[1] = false; // White Q-side rook captured at a1
            if (m.after.x == 7 && m.after.y == 7) pos->c[0] = false; // White K-side rook captured at h1
        }
    }
}

// Helper to update en passant target square
static void update_en_passant_target(ChessPosition *pos, Move m) {
    // (a) Clear default: no en passant target for the next turn unless a pawn makes a double move
    pos->e[0] = -1;
    pos->e[1] = -1;

    char moved_piece_upper = toupper(m.piece_moved);

    // (b) If a pawn moves two squares forward, set the en passant target.
    if (moved_piece_upper == 'P') {
        if (m.piece_moved == 'P' && m.before.y == 6 && m.after.y == 4) { // White pawn moves from rank 2 (y=6) to rank 4 (y=4)
            pos->e[0] = m.after.x;
            pos->e[1] = 5; // Target square is behind the pawn on rank 3 (y=5)
        } else if (m.piece_moved == 'p' && m.before.y == 1 && m.after.y == 3) { // Black pawn moves from rank 7 (y=1) to rank 5 (y=3)
            pos->e[0] = m.after.x;
            pos->e[1] = 2; // Target square is behind the pawn on rank 6 (y=2)
        }
    }
}


void move(Move inputMove, ChessPosition *pos)
{
    pos->lastMove = inputMove;

    char actual_captured_piece_on_board = pos->b[inputMove.after.x][inputMove.after.y];

    pos->b[inputMove.after.x][inputMove.after.y] = inputMove.piece_moved;
    pos->b[inputMove.before.x][inputMove.before.y] = '-';

    if (inputMove.is_castling)
    {
        if (inputMove.piece_moved == 'K') // White King
        {
            if (inputMove.after.x == 6) // Kingside (e1-g1, rook h1-f1)
            {
                pos->b[5][7] = 'R'; // Move rook from h1 (7,7) to f1 (5,7)
                pos->b[7][7] = '-'; // Clear h1
            }
            else if (inputMove.after.x == 2) // Queenside (e1-c1, rook a1-d1)
            {
                pos->b[3][7] = 'R'; // Move rook from a1 (0,7) to d1 (3,7)
                pos->b[0][7] = '-'; // Clear a1
            }
        }
        else if (inputMove.piece_moved == 'k') // Black King
        {
            if (inputMove.after.x == 6) // Kingside (e8-g8, rook h8-f8)
            {
                pos->b[5][0] = 'r'; // Move rook from h8 (7,0) to f8 (5,0)
                pos->b[7][0] = '-'; // Clear h8
            }
            else if (inputMove.after.x == 2) // Queenside (e8-c8, rook a8-d8)
            {
                pos->b[3][0] = 'r'; // Move rook from a8 (0,0) to d8 (3,0)
                pos->b[0][0] = '-'; // Clear a8
            }
        }
    }

    if (inputMove.is_en_passant)
    {
        if (inputMove.piece_moved == 'P') // White pawn captured black pawn
        {
            pos->b[inputMove.after.x][inputMove.after.y + 1] = '-'; // e.g., if White to d6 (x=3,y=2), remove pawn at d5 (x=3,y=3)
        }
        else if (inputMove.piece_moved == 'p') // Black pawn captured white pawn
        {
            pos->b[inputMove.after.x][inputMove.after.y - 1] = '-'; // e.g., if Black to d3 (x=3,y=5), remove pawn at d4 (x=3,y=4)
        }
    }

    if (inputMove.promotion_piece != '\0')
    {
        pos->b[inputMove.after.x][inputMove.after.y] = inputMove.promotion_piece;
    }

    update_castling_rights_after_move(pos, inputMove, actual_captured_piece_on_board);
    update_en_passant_target(pos, inputMove);

    bool isPawnMove = (toupper(inputMove.piece_moved) == 'P');
    bool isCapture = (inputMove.captured_piece != '-'); // Use the captured_piece from the Move struct

    if (isPawnMove || isCapture)
    {
        pos->m[0] = 0; // Reset halfmove clock
    }
    else
    {
        pos->m[0]++; // Increment halfmove clock
    }

    pos->w = !pos->w;
    if (pos->w) // If it's now White's turn, it means Black just moved
    {
        pos->m[1]++;
    }

    printf("\n--- Move Details ---\n");
    printf("Move Applied: %c%d to %c%d, Piece: %c\n",
           'a' + inputMove.before.x, 8 - inputMove.before.y,
           'a' + inputMove.after.x, 8 - inputMove.after.y,
           inputMove.piece_moved);
    printf("Captured: '%c', Promoted: '%c', Castling: %d, En Passant: %d\n",
           inputMove.captured_piece, inputMove.promotion_piece,
           inputMove.is_castling, inputMove.is_en_passant);
    printf("New castling rights: WK: %d, WQ: %d, BK: %d, BQ: %d\n",
           pos->c[0], pos->c[1], pos->c[2], pos->c[3]);
    printf("New En Passant target: %c%d\n",
           pos->e[0] != -1 ? ('a' + pos->e[0]) : '-',
           pos->e[1] != -1 ? (8 - pos->e[1]) : '-');
    printf("Halfmove clock: %d, Fullmove number: %d\n", pos->m[0], pos->m[1]);
    printf("Side to move is now: %s\n", pos->w ? "White" : "Black");
    printf("--------------------\n");
}