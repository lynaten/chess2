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

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h> // for toupper, tolower, etc.

void move(Move inputMove, ChessPosition *pos)
{
    // Convert from algebraic-like input to array indexes
    int fromFile = inputMove.before.x; // 'a' -> 0, 'h' -> 7
    int fromRank = inputMove.before.y; // rank 1 => row 7, rank 8 => row 0
    int toFile = inputMove.after.x;
    int toRank = inputMove.after.y;

    // 1) Identify the moving piece
    char piece = pos->b[fromFile][fromRank];

    // 2) Identify if we're capturing a piece
    char captured = pos->b[toFile][toRank]; // occupant of the 'to' square BEFORE we move

    pos->lastMove = inputMove;
    // ------------------------------------------
    // Special Case A: Castling
    // ------------------------------------------
    // Castling is recognized by the king moving exactly 2 squares left/right from its start.
    // White King normally at (4,7). Black King at (4,0).
    // If it's the king, check if abs(toFile - fromFile) == 2 and same rank.

    bool castling = false;
    if ((piece == 'K' && fromFile == 4 && fromRank == 7 && fromRank == toRank && (toFile == 2 || toFile == 6)) ||
        (piece == 'k' && fromFile == 4 && fromRank == 0 && fromRank == toRank && (toFile == 2 || toFile == 6)))
    {
        castling = true;
    }

    // ------------------------------------------
    // 3) Execute the move on the board (initially)
    // ------------------------------------------
    pos->b[toFile][toRank] = piece;
    pos->b[fromFile][fromRank] = '-';

    // ------------------------------------------
    // Special Case A continued: Actually place the rook for castling
    // ------------------------------------------
    if (castling)
    {
        // If White’s King
        if (piece == 'K')
        {
            // King is going from e1 => c1 or g1
            // e1 is (4,7). c1 is (2,7). g1 is (6,7).
            if (toFile == 2)
            {
                // Queenside castling: Move the rook from a1 => d1
                // a1 => (0,7), d1 => (3,7)
                pos->b[3][7] = 'R';
                pos->b[0][7] = '-';
            }
            else
            {
                // Kingside castling: Move the rook from h1 => f1
                // h1 => (7,7), f1 => (5,7)
                pos->b[5][7] = 'R';
                pos->b[7][7] = '-';
            }
        }
        // If Black’s King
        else if (piece == 'k')
        {
            // e8 => c8 or g8
            // e8 => (4,0), c8 => (2,0), g8 => (6,0)
            if (toFile == 2)
            {
                // Queenside castling: Move the rook from a8 => d8
                // a8 => (0,0), d8 => (3,0)
                pos->b[3][0] = 'r';
                pos->b[0][0] = '-';
            }
            else
            {
                // Kingside castling: Move the rook from h8 => f8
                // h8 => (7,0), f8 => (5,0)
                pos->b[5][0] = 'r';
                pos->b[7][0] = '-';
            }
        }
    }

    // ------------------------------------------
    // 4) Update castling rights
    // ------------------------------------------
    // If a king moves at all, remove castling for that side.
    // If a rook moves from its start, remove castling for that side.
    // Also if you capture an enemy rook on a corner, remove that side's castling.
    if (piece == 'K')
    {
        pos->c[0] = false; // white kingside
        pos->c[1] = false; // white queenside
    }
    else if (piece == 'k')
    {
        pos->c[2] = false; // black kingside
        pos->c[3] = false; // black queenside
    }
    else if (piece == 'R')
    {
        if (fromFile == 0 && fromRank == 7)
            pos->c[1] = false; // white queenside
        if (fromFile == 7 && fromRank == 7)
            pos->c[0] = false; // white kingside
    }
    else if (piece == 'r')
    {
        if (fromFile == 0 && fromRank == 0)
            pos->c[3] = false; // black queenside
        if (fromFile == 7 && fromRank == 0)
            pos->c[2] = false; // black kingside
    }

    // *Additionally*, if we captured a rook in its original corner, remove that side’s castling.
    if (captured == 'R')
    {
        // White rook captured on A1 => remove whiteQueenside, or on H1 => remove whiteKingside
        if (toFile == 0 && toRank == 7)
            pos->c[1] = false;
        if (toFile == 7 && toRank == 7)
            pos->c[0] = false;
    }
    else if (captured == 'r')
    {
        // Black rook captured on A8 => remove blackQueenside, or on H8 => remove blackKingside
        if (toFile == 0 && toRank == 0)
            pos->c[3] = false;
        if (toFile == 7 && toRank == 0)
            pos->c[2] = false;
    }

    // ------------------------------------------
    // 5) Handle en passant
    // ------------------------------------------
    //
    // (a) Clear default: no en passant
    pos->e[0] = -1;
    pos->e[1] = -1;

    // (b) If a pawn moves two squares forward from its start, set the en passant target.
    if (piece == 'P' && fromRank == 6 && toRank == 4)
    {
        pos->e[0] = toFile;
        pos->e[1] = 5;
    }
    else if (piece == 'p' && fromRank == 1 && toRank == 3)
    {
        pos->e[0] = toFile;
        pos->e[1] = 2;
    }

    // (c) En passant *capture* detection:
    // If a pawn moves diagonally onto an empty square, that means it's an en passant capture.
    // White en passant: from rank=3 to rank=2, black’s pawn must have been on (toFile, 3).
    bool wasEmptyCapture = (captured == '-' && (piece == 'P' || piece == 'p') && (toFile != fromFile));
    if (wasEmptyCapture)
    {
        // White capturing black en passant
        if (piece == 'P' && fromRank == 3 && toRank == 2)
        {
            // The captured black pawn is behind the 'to' square => (toFile, 3)
            pos->b[toFile][3] = '-';
        }
        // Black capturing white en passant
        else if (piece == 'p' && fromRank == 4 && toRank == 5)
        {
            // The captured white pawn was behind the 'to' square => (toFile, 4)
            pos->b[toFile][4] = '-';
        }
    }

    // ------------------------------------------
    // 6) Promotion
    // ------------------------------------------
    // If a white pawn hits rank=0 or black pawn hits rank=7, we promote to a queen by default.
    if (piece == 'P' && toRank == 0)
    {
        // By default, let’s do a queen promotion
        pos->b[toFile][toRank] = 'Q';
    }
    else if (piece == 'p' && toRank == 7)
    {
        pos->b[toFile][toRank] = 'q';
    }

    // ------------------------------------------
    // 7) Update half‐move clock
    // ------------------------------------------
    bool isPawn = (piece == 'P' || piece == 'p');
    bool isCapture = (captured != '-'); // was there something on the target square?

    // Pawn move OR capture => reset halfmove
    // (Note that en passant captures also get handled here because it's still a capture.)
    if (isPawn || isCapture)
    {
        pos->m[0] = 0;
    }
    else
    {
        pos->m[0]++;
    }

    // ------------------------------------------
    // 8) Flip side to move
    // ------------------------------------------
    pos->w = !pos->w;

    // If we've just changed pos->w to true, that means Black has completed a move => increment fullmove
    if (pos->w)
    {
        pos->m[1]++;
    }

    // Debug
    // printf("Castling rights: White K=%d Q=%d  Black K=%d Q=%d\n",
    //        pos->c[0], pos->c[1], pos->c[2], pos->c[3]);
    // printf("En Passant: [%d, %d]\n", pos->e[0], pos->e[1]);
    // printf("Side to move is now: %s\n", pos->w ? "White" : "Black");
    // printf("Halfmove: %d, Fullmove: %d\n", pos->m[0], pos->m[1]);
}
