#include "header/chess/space.h"

int space_area(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = pos1;
    if (square == NULL)
    {
        return sum(pos, space_area, NULL);
    }

    int v = 0;
    int rankVar = rank(pos, square, NULL);
    int fileVar = file(pos, square, NULL);

    if ((rankVar >= 2 && rankVar <= 4 && fileVar >= 3 && fileVar <= 6) &&
        (board(pos, square->x, square->y) != 'P') &&
        (board(pos, square->x - 1, square->y - 1) != 'p') &&
        (board(pos, square->x + 1, square->y - 1) != 'p'))
    {
        v++;
        if ((board(pos, square->x, square->y - 1) == 'P' ||
             board(pos, square->x, square->y - 2) == 'P' ||
             board(pos, square->x, square->y - 3) == 'P') &&
            !attack(pos2, &(Square){square->x, 7 - square->y}, NULL))
        {
            v++;
        }
    }
    return v;
}

int pieceCount(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, piece_count, NULL);
    }
    char piece = board(pos, square->x, square->y);
    if (strchr("PNBRQK", piece) != NULL)
    {
        return 1;
    }
    return 0;
}

int blockedCount(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, blockedCount, NULL);
    }
    int result = 0;
    char piece = board(pos, square->x, square->y);
    if (piece == 'P' &&
        (board(pos, square->x, square->y - 1) == 'p' ||
         (board(pos, square->x - 1, square->y - 2) == 'p' &&
          board(pos, square->x + 1, square->y - 2) == 'p')))
    {
        result++;
    }
    if (piece == 'p' &&
        (board(pos, square->x, square->y + 1) == 'P' ||
         (board(pos, square->x - 1, square->y + 2) == 'P' &&
          board(pos, square->x + 1, square->y + 2) == 'P')))
    {
        result++;
    }
    return result;
}

int space(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    ChessPosition *pos3 = !colorflipped ? pos2 : pos1;

    if (non_pawn_material(pos, NULL, NULL) + non_pawn_material(pos3, NULL, NULL) < 12222)
    {
        return 0;
    }
    int pieceCount = 0;
    int blockedCount = 0;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            char piece = board(pos, x, y);
            if (strchr("PNBRQK", piece) != NULL)
            {
                pieceCount++;
            }
            if (piece == 'P' &&
                (board(pos, x, y - 1) == 'p' ||
                 (board(pos, x - 1, y - 2) == 'p' &&
                  board(pos, x + 1, y - 2) == 'p')))
            {
                blockedCount++;
            }
            if (piece == 'p' &&
                (board(pos, x, y + 1) == 'P' ||
                 (board(pos, x - 1, y + 2) == 'P' &&
                  board(pos, x + 1, y + 2) == 'P')))
            {
                blockedCount++;
            }
        }
    }

    int weight = pieceCount - 3 + MIN(blockedCount, 9);
    return ((space_area(pos, NULL, NULL) * weight * weight / 16) << 0);
}
