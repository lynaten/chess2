#include "header/chess/passedPawn.h"

int candidate_passed(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, candidate_passed, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }

    int ty1 = 8, ty2 = 8;
    for (int y = square->y - 1; y >= 0; y--)
    {
        if (board(pos, square->x, y) == 'P')
            return 0;
        if (board(pos, square->x, y) == 'p')
            ty1 = y;
        if (board(pos, square->x - 1, y) == 'p' || board(pos, square->x + 1, y) == 'p')
            ty2 = y;
    }

    if (ty1 == 8 && ty2 >= square->y - 1)
        return 1;
    if (ty2 < square->y - 2 || ty1 < square->y - 1)
        return 0;

    if (ty2 >= square->y && ty1 == square->y - 1 && square->y < 4)
    {
        if (board(pos, square->x - 1, square->y + 1) == 'P' &&
            board(pos, square->x - 1, square->y) != 'p' &&
            board(pos, square->x - 2, square->y - 1) != 'p')
            return 1;

        if (board(pos, square->x + 1, square->y + 1) == 'P' &&
            board(pos, square->x + 1, square->y) != 'p' &&
            board(pos, square->x + 2, square->y - 1) != 'p')
            return 1;
    }

    if (board(pos, square->x, square->y - 1) == 'p')
        return 0;

    int lever = (board(pos, square->x - 1, square->y - 1) == 'p') +
                (board(pos, square->x + 1, square->y - 1) == 'p');

    int leverpush = (board(pos, square->x - 1, square->y - 2) == 'p') +
                    (board(pos, square->x + 1, square->y - 2) == 'p');

    int phalanx = (board(pos, square->x - 1, square->y) == 'P') +
                  (board(pos, square->x + 1, square->y) == 'P');

    if (lever - supported(pos, square, NULL) > 1)
        return 0;
    if (leverpush - phalanx > 0)
        return 0;
    if (lever > 0 && leverpush > 0)
        return 0;

    return 1;
}

int king_proximity(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, king_proximity, NULL);
    }
    if (!passed_leverable(pos, square, NULL))
    {
        return 0;
    }
    int r = rank(pos, square, NULL) - 1;
    int w = r > 2 ? 5 * r - 13 : 0;
    if (w <= 0)
    {
        return 0;
    }

    int v = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'k')
            {
                v += ((MIN(MAX(abs(y - square->y + 1), abs(x - square->x)), 5) * 19 / 4) * w);
            }
            if (board(pos, x, y) == 'K')
            {
                v -= MIN(MAX(abs(y - square->y + 1), abs(x - square->x)), 5) * 2 * w;
                if (square->y > 1)
                {
                    v -= MIN(MAX(abs(y - square->y + 2), abs(x - square->x)), 5) * w;
                }
            }
        }
    }
    return v;
}

int passed_block(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = pos1;
    if (square == NULL)
    {
        return sum(pos, passed_block, NULL);
    }
    if (!passed_leverable(pos, square, NULL))
    {
        return 0;
    }
    if (rank(pos, square, NULL) < 4)
    {
        return 0;
    }
    if (board(pos, square->x, square->y - 1) != '-')
    {
        return 0;
    }
    int r = rank(pos, square, NULL) - 1;
    int w = r > 2 ? 5 * r - 13 : 0;

    int defended = 0, unsafe = 0, wunsafe = 0, defended1 = 0, unsafe1 = 0;

    for (int y = square->y - 1; y >= 0; y--)
    {
        if (attack(pos, &(Square){square->x, y}, NULL))
            defended++;
        if (attack(pos2, &(Square){square->x, 7 - y}, NULL))
            unsafe++;
        if (attack(pos2, &(Square){square->x - 1, 7 - y}, NULL))
            wunsafe++;
        if (attack(pos2, &(Square){square->x + 1, 7 - y}, NULL))
            wunsafe++;
        if (y == square->y - 1)
        {
            defended1 = defended;
            unsafe1 = unsafe;
        }
    }

    for (int y = square->y + 1; y < 8; y++)
    {
        if (board(pos, square->x, y) == 'R' || board(pos, square->x, y) == 'Q')
            defended1 = defended = square->y;
        if (board(pos, square->x, y) == 'r' || board(pos, square->x, y) == 'q')
            unsafe1 = unsafe = square->y;
    }

    int k = (unsafe == 0 && wunsafe == 0 ? 35 : unsafe == 0 ? 20
                                            : unsafe1 == 0  ? 9
                                                            : 0) +
            (defended1 != 0 ? 5 : 0);

    return k * w;
}

int passed_file(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, passed_file, NULL);
    }
    if (!passed_leverable(pos, square, NULL))
    {
        return 0;
    }
    int file_num = file(pos, square, NULL);
    return MIN(file_num - 1, 8 - file_num);
}

int passed_rank(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, passed_rank, NULL);
    }
    if (!passed_leverable(pos, square, NULL))
    {
        return 0;
    }
    return rank(pos, square, NULL) - 1;
}

int passed_leverable(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = pos1;
    if (square == NULL)
    {
        return sum(pos, passed_leverable, NULL);
    }
    if (!candidate_passed(pos, square, NULL))
    {
        return 0;
    }
    if (board(pos, square->x, square->y - 1) != 'p')
    {
        return 1;
    }

    for (int i = -1; i <= 1; i += 2)
    {
        Square s1 = {square->x + i, square->y};
        Square s2 = {square->x + i, 7 - square->y};

        if (board(pos, square->x + i, square->y + 1) == 'P' &&
            strchr("pnbrqk", board(pos, square->x + i, square->y)) == NULL &&
            (attack(pos, &s1, NULL) > 0 || attack(pos2, &s2, NULL) <= 1))
        {
            return 1;
        }
    }

    return 0;
}

int passed_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, passed_mg, NULL);
    }
    if (!passed_leverable(pos, square, NULL))
    {
        return 0;
    }

    int passed_rank_bonus[] = {0, 10, 17, 15, 62, 168, 276};
    int v = 0;
    v += passed_rank_bonus[passed_rank(pos, square, NULL)];
    v += passed_block(pos, square, NULL);
    v -= 11 * passed_file(pos, square, NULL);

    return v;
}

int passed_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, passed_eg, NULL);
    }
    if (!passed_leverable(pos, square, NULL))
    {
        return 0;
    }

    int passed_rank_bonus[] = {0, 28, 33, 41, 72, 177, 260};
    int v = 0;
    v += king_proximity(pos, square, NULL);
    v += passed_rank_bonus[passed_rank(pos, square, NULL)];
    v += passed_block(pos, square, NULL);
    v -= 8 * passed_file(pos, square, NULL);

    return v;
}
