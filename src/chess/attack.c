#include "header/chess/attack.h"

int pinned_direction(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, pinned_direction, NULL);
    char piece = board(pos, square->x, square->y);
    if (!strchr("PNBRQK", piece))
        return 0;
    int color = strchr("PNBRQK", piece) ? 1 : -1;

    for (int i = 0; i < 8; i++)
    {
        int ix = (i + (i > 3)) % 3 - 1;
        int iy = ((i + (i > 3)) / 3) - 1;
        bool king = false;

        for (int d = 1; d < 8; d++)
        {
            char b = board(pos, square->x + d * ix, square->y + d * iy);
            if (b == 'K')
                king = true;
            if (b != '-')
                break;
        }

        if (king)
        {
            for (int d = 1; d < 8; d++)
            {
                char b = board(pos, square->x - d * ix, square->y - d * iy);
                if (b == 'q' || (b == 'b' && ix * iy != 0) || (b == 'r' && ix * iy == 0))
                    return abs(ix + iy * 3) * color;
                if (b != '-')
                    break;
            }
        }
    }
    return 0;
}

int knight_attack(ChessPosition *pos, Square *square, void *param)
{
    Square *s2 = (Square *)param;
    if (!square)
        return sum(pos, knight_attack, NULL);
    int v = 0;
    for (int i = 0; i < 8; i++)
    {
        int ix = ((i > 3) + 1) * (((i % 4) > 1) * 2 - 1);
        int iy = (2 - (i > 3)) * ((i % 2 == 0) * 2 - 1);
        char b = board(pos, square->x + ix, square->y + iy);
        if (b == 'N' && (!s2 || (s2->x == square->x + ix && s2->y == square->y + iy)) && !pinned(pos, &(Square){square->x + ix, square->y + iy}, NULL))
            v++;
    }
    return v;
}

int bishop_xray_attack(ChessPosition *pos, Square *square, void *param)
{
    Square *s2 = (Square *)param;
    if (!square)
        return sum(pos, bishop_xray_attack, NULL);
    int v = 0;
    for (int i = 0; i < 4; i++)
    {
        int ix = ((i > 1) * 2 - 1);
        int iy = ((i % 2 == 0) * 2 - 1);
        for (int d = 1; d < 8; d++)
        {
            char b = board(pos, square->x + d * ix, square->y + d * iy);
            if (b == 'B' && (!s2 || (s2->x == square->x + d * ix && s2->y == square->y + d * iy)))
            {
                int dir = pinned_direction(pos, &(Square){square->x + d * ix, square->y + d * iy}, NULL);
                if (dir == 0 || abs(ix + iy * 3) == dir)
                    v++;
            }
            if (b != '-' && b != 'Q' && b != 'q')
                break;
        }
    }
    return v;
}

int rook_xray_attack(ChessPosition *pos, Square *square, void *param)
{
    Square *s2 = (Square *)param;
    if (!square)
        return sum(pos, rook_xray_attack, NULL);
    int v = 0;
    for (int i = 0; i < 4; i++)
    {
        int ix = (i == 0 ? -1 : i == 1 ? 1
                                       : 0);
        int iy = (i == 2 ? -1 : i == 3 ? 1
                                       : 0);
        for (int d = 1; d < 8; d++)
        {
            char b = board(pos, square->x + d * ix, square->y + d * iy);
            if (b == 'R' && (!s2 || (s2->x == square->x + d * ix && s2->y == square->y + d * iy)))
            {
                int dir = pinned_direction(pos, &(Square){square->x + d * ix, square->y + d * iy}, NULL);
                if (dir == 0 || abs(ix + iy * 3) == dir)
                    v++;
            }
            if (b != '-' && b != 'R' && b != 'Q' && b != 'q')
                break;
        }
    }
    return v;
}

int queen_attack(ChessPosition *pos, Square *square, void *param)
{
    Square *s2 = (Square *)param;
    if (!square)
        return sum(pos, queen_attack, NULL);
    int v = 0;
    for (int i = 0; i < 8; i++)
    {
        int ix = (i + (i > 3)) % 3 - 1;
        int iy = ((i + (i > 3)) / 3) - 1;
        for (int d = 1; d < 8; d++)
        {
            char b = board(pos, square->x + d * ix, square->y + d * iy);
            if (b == 'Q' && (!s2 || (s2->x == square->x + d * ix && s2->y == square->y + d * iy)))
            {
                int dir = pinned_direction(pos, &(Square){square->x + d * ix, square->y + d * iy}, NULL);
                if (dir == 0 || abs(ix + iy * 3) == dir)
                    v++;
            }
            if (b != '-')
                break;
        }
    }
    return v;
}

int pawn_attack(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, pawn_attack, NULL);
    int v = 0;
    if (board(pos, square->x - 1, square->y + 1) == 'P')
        v++;
    if (board(pos, square->x + 1, square->y + 1) == 'P')
        v++;
    return v;
}

int king_attack(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, king_attack, NULL);
    for (int i = 0; i < 8; i++)
    {
        int ix = (i + (i > 3)) % 3 - 1;
        int iy = ((i + (i > 3)) / 3) - 1;
        if (board(pos, square->x + ix, square->y + iy) == 'K')
            return 1;
    }
    return 0;
}

int attack(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
        return sum(pos, attack, NULL);
    int v = 0;
    v += pawn_attack(pos, square, NULL);
    v += king_attack(pos, square, NULL);
    v += knight_attack(pos, square, NULL);
    v += bishop_xray_attack(pos, square, NULL);
    v += rook_xray_attack(pos, square, NULL);
    v += queen_attack(pos, square, NULL);
    return v;
}

bool is_square_attacked(ChessPosition *pos, int x, int y, bool byWhite)
{
    bool original = pos->w;
    pos->w = byWhite; // Pretend it's the enemy's turn

    Square s = {x, y};
    int attackers = attack(pos, &s, NULL);

    pos->w = original; // Restore original side
    return attackers > 0;
}

// Queen diagonal attack function
int queen_attack_diagonal(ChessPosition *pos, Square *square, void *param)
{
    Square *s2 = (Square *)param;
    if (!square)
    {
        return sum(pos, queen_attack_diagonal, NULL); // Assuming `sum` is implemented elsewhere
    }

    int v = 0;
    for (int i = 0; i < 8; i++)
    {
        int ix = (i + (i > 3)) % 3 - 1;
        int iy = ((i + (i > 3)) / 3) - 1;

        if (ix == 0 || iy == 0)
        {
            continue;
        }

        for (int d = 1; d < 8; d++)
        {
            char b = board(pos, square->x + d * ix, square->y + d * iy);

            if (b == 'Q' && (!s2 || (s2->x == square->x + d * ix && s2->y == square->y + d * iy)))
            {
                int dir = pinned_direction(pos, &(Square){square->x + d * ix, square->y + d * iy}, NULL);

                if (dir == 0 || abs(ix + iy * 3) == dir)
                {
                    v++;
                }
            }

            if (b != '-')
            {
                break;
            }
        }
    }

    return v;
}

// Pinned function
int pinned(ChessPosition *pos, Square *square, void *param)
{
    if (!square)
    {
        return sum(pos, pinned, NULL); // Assuming `sum` is implemented elsewhere
    }

    char piece = board(pos, square->x, square->y);

    // Check if the piece is one of "PNBRQK"
    if (!strchr("PNBRQK", piece))
    {
        return 0;
    }

    return pinned_direction(pos, square, NULL) > 0 ? 1 : 0;
}
