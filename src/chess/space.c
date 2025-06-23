#include "header/chess/space.h"

int space_area(ChessPosition *pos, Square *square, void *param)
{
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
        ChessPosition flipped;
        ChessPosition *pos2 = &flipped;
        colorflip(pos, pos2);
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

int pieceCount(ChessPosition *pos, Square *square, void *param)
{
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

int blockedCount(ChessPosition *pos, Square *square, void *param)
{
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

int space(ChessPosition *pos)
{
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);

    pos->eval.non_pawn_material = non_pawn_material(pos, NULL, NULL) + non_pawn_material(pos2, NULL, NULL);

    if (pos->eval.non_pawn_material < 12222)
    {
        return 0;
    }

    pos->eval.piece_count = pieceCount(pos, NULL, NULL);
    pos->eval.blocked_count = blockedCount(pos, NULL, NULL);
    pos->eval.space_area = space_area(pos, NULL, NULL);

    int weight = pos->eval.piece_count - 3 + MIN(pos->eval.blocked_count, 9);
    int result = ((pos->eval.space_area * weight * weight / 16) << 0);

    return result;
}
