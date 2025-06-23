#include "header/chess/helper.h"

// Function to return the rank of a square
int rank(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, rank, NULL);
    return 8 - square->y;
}

// Function to return the file of a square
int file(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, file, NULL);
    return 1 + square->x;
}

// Function to count the number of bishops
int bishop_count(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, bishop_count, NULL);
    return board(pos, square->x, square->y) == 'B' ? 1 : 0;
}

// Function to count the number of queens
int queen_count(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, queen_count, NULL);
    return board(pos, square->x, square->y) == 'Q' ? 1 : 0;
}

// Function to count the number of pawns
int pawn_count(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, pawn_count, NULL);
    return board(pos, square->x, square->y) == 'P' ? 1 : 0;
}

// Function to count the number of knights
int knight_count(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, knight_count, NULL);
    return board(pos, square->x, square->y) == 'N' ? 1 : 0;
}

// Function to count the number of rooks
int rook_count(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, rook_count, NULL);
    return board(pos, square->x, square->y) == 'R' ? 1 : 0;
}

// Function to determine if there are opposite bishops
int opposite_bishops(ChessPosition *pos)
{
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    if (bishop_count(pos, NULL, NULL) != 1 || bishop_count(pos2, NULL, NULL) != 1)
    {
        return 0;
    }

    int color[2] = {0, 0};
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            char piece = board(pos, x, y);
            if (piece == 'B')
                color[0] = (x + y) % 2;
            if (piece == 'b')
                color[1] = (x + y) % 2;
        }
    }
    return color[0] != color[1];
}

// Function to calculate the king's distance from a square
int king_distance(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, king_distance, NULL);

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'K')
            {
                return fmax(abs(x - square->x), abs(y - square->y));
            }
        }
    }
    return 0;
}

// Function to check if a square is in the king's ring
int king_ring(ChessPosition *pos, Square *square, void *param)
{
    int full = *(int *)param;
    if (!square)
        return sum(pos, king_ring, param);

    if (!full && board(pos, square->x + 1, square->y - 1) == 'p' && board(pos, square->x - 1, square->y - 1) == 'p')
    {
        return 0;
    }

    for (int ix = -2; ix <= 2; ix++)
    {
        for (int iy = -2; iy <= 2; iy++)
        {
            if (board(pos, square->x + ix, square->y + iy) == 'k' && (ix >= -1 && ix <= 1 || square->x + ix == 0 || square->x + ix == 7) && (iy >= -1 && iy <= 1 || square->y + iy == 0 || square->y + iy == 7))
            {
                return 1;
            }
        }
    }
    return 0;
}

// Function to count the total number of pieces
int piece_count(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, piece_count, NULL);

    char piece = board(pos, square->x, square->y);
    return (strchr("PNBRQK", piece) != NULL) ? 1 : 0;
}

// Function to calculate pawn attacks span
int pawn_attacks_span(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, pawn_attacks_span, NULL);

    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    for (int y = 0; y < square->y; y++)
    {
        if (board(pos, square->x - 1, y) == 'p' && (y == square->y - 1 || (board(pos, square->x - 1, y + 1) != 'P' && !backward(pos2, &(Square){square->x - 1, 7 - y}, NULL))))
        {
            return 1;
        }
        if (board(pos, square->x + 1, y) == 'p' && (y == square->y - 1 || (board(pos, square->x + 1, y + 1) != 'P' && !backward(pos2, &(Square){square->x + 1, 7 - y}, NULL))))
        {
            return 1;
        }
    }
    return 0;
}
