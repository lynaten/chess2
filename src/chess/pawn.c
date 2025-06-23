#include "header/chess/pawn.h"
int isolated(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, isolated, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    for (int y = 0; y < 8; y++)
    {
        if (board(pos, square->x - 1, y) == 'P' || board(pos, square->x + 1, y) == 'P')
        {
            return 0;
        }
    }
    return 1;
}

int opposed(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, opposed, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    for (int y = 0; y < square->y; y++)
    {
        if (board(pos, square->x, y) == 'p')
        {
            return 1;
        }
    }
    return 0;
}

int phalanx(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, phalanx, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    if (board(pos, square->x - 1, square->y) == 'P' || board(pos, square->x + 1, square->y) == 'P')
    {
        return 1;
    }
    return 0;
}

int supported(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, supported, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    return (board(pos, square->x - 1, square->y + 1) == 'P' ? 1 : 0) + (board(pos, square->x + 1, square->y + 1) == 'P' ? 1 : 0);
}

int backward(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, backward, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    for (int y = square->y; y < 8; y++)
    {
        if (board(pos, square->x - 1, y) == 'P' || board(pos, square->x + 1, y) == 'P')
        {
            return 0;
        }
    }
    if (board(pos, square->x - 1, square->y - 2) == 'p' ||
        board(pos, square->x + 1, square->y - 2) == 'p' ||
        board(pos, square->x, square->y - 1) == 'p')
    {
        return 1;
    }
    return 0;
}

int doubled(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, doubled, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    if (board(pos, square->x, square->y + 1) != 'P')
    {
        return 0;
    }
    if (board(pos, square->x - 1, square->y + 1) == 'P')
    {
        return 0;
    }
    if (board(pos, square->x + 1, square->y + 1) == 'P')
    {
        return 0;
    }
    return 1;
}

int connected(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, connected, NULL);
    }
    if (supported(pos, square, NULL) || phalanx(pos, square, NULL))
    {
        return 1;
    }
    return 0;
}

int connected_bonus(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, connected_bonus, NULL);
    }
    if (!connected(pos, square, NULL))
    {
        return 0;
    }

    int seed[] = {0, 7, 8, 12, 29, 48, 86};
    int op = opposed(pos, square, NULL);
    int ph = phalanx(pos, square, NULL);
    int su = supported(pos, square, NULL);
    int bl = board(pos, square->x, square->y - 1) == 'p' ? 1 : 0;
    int r = rank(pos, square, NULL);

    if (r < 2 || r > 7)
    {
        return 0;
    }

    return seed[r - 1] * (2 + ph - op) + 21 * su;
}

int weak_unopposed_pawn(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, weak_unopposed_pawn, NULL);
    }
    if (opposed(pos, square, NULL))
    {
        return 0;
    }

    int v = 0;
    if (isolated(pos, square, NULL))
    {
        v++;
    }
    else if (backward(pos, square, NULL))
    {
        v++;
    }

    return v;
}

int weak_lever(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, weak_lever, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    if (board(pos, square->x - 1, square->y - 1) != 'p')
    {
        return 0;
    }
    if (board(pos, square->x + 1, square->y - 1) != 'p')
    {
        return 0;
    }
    if (board(pos, square->x - 1, square->y + 1) == 'P')
    {
        return 0;
    }
    if (board(pos, square->x + 1, square->y + 1) == 'P')
    {
        return 0;
    }
    return 1;
}

int blocked(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, blocked, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    if (square->y != 2 && square->y != 3)
    {
        return 0;
    }
    if (board(pos, square->x, square->y - 1) != 'p')
    {
        return 0;
    }
    return 4 - square->y;
}

int doubled_isolated(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, doubled_isolated, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    if (isolated(pos, square, NULL))
    {
        int obe = 0, eop = 0, ene = 0;
        for (int y = 0; y < 8; y++)
        {
            if (y > square->y && board(pos, square->x, y) == 'P')
            {
                obe++;
            }
            if (y < square->y && board(pos, square->x, y) == 'p')
            {
                eop++;
            }
            if (board(pos, square->x - 1, y) == 'p' || board(pos, square->x + 1, y) == 'p')
            {
                ene++;
            }
        }
        if (obe > 0 && ene == 0 && eop > 0)
        {
            return 1;
        }
    }
    return 0;
}

int pawns_mg(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, pawns_mg, NULL);
    }
    int v = 0;
    if (doubled_isolated(pos, square, NULL))
    {
        v -= 11;
    }
    else if (isolated(pos, square, NULL))
    {
        v -= 5;
    }
    else if (backward(pos, square, NULL))
    {
        v -= 9;
    }
    v -= doubled(pos, square, NULL) * 11;
    v += connected(pos, square, NULL) ? connected_bonus(pos, square, NULL) : 0;
    v -= 13 * weak_unopposed_pawn(pos, square, NULL);
    int blocked_value = blocked(pos, square, NULL);
    v += (blocked_value == 0 ? 0 : blocked_value == 1 ? -11
                                                      : -3);
    return v;
}

int pawns_eg(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, pawns_eg, NULL);
    }
    int v = 0;
    if (doubled_isolated(pos, square, NULL))
    {
        v -= 56;
    }
    else if (isolated(pos, square, NULL))
    {
        v -= 15;
    }
    else if (backward(pos, square, NULL))
    {
        v -= 24;
    }
    v -= doubled(pos, square, NULL) * 56;
    if (connected(pos, square, NULL))
    {
        v += connected_bonus(pos, square, NULL) * (rank(pos, square, NULL) - 3) / 4;
    }
    v -= 27 * weak_unopposed_pawn(pos, square, NULL);
    v -= 56 * weak_lever(pos, square, NULL);
    int blocked_value = blocked(pos, square, NULL);
    v += (blocked_value == 0 ? 0 : blocked_value == 1 ? -4
                                                      : 4);
    return v;
}
