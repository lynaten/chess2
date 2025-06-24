#include "header/chess/piece.h"

int outpost(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, outpost, NULL);
    }
    char piece = board(pos, square->x, square->y);
    if (piece != 'N' && piece != 'B')
    {
        return 0;
    }
    if (!outpost_square(pos, square, NULL))
    {
        return 0;
    }
    return 1;
}

int outpost_square(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, outpost_square, NULL);
    }
    int r = rank(pos, square, NULL);
    if (r < 4 || r > 6)
    {
        return 0;
    }
    if (board(pos, square->x - 1, square->y + 1) != 'P' &&
        board(pos, square->x + 1, square->y + 1) != 'P')
    {
        return 0;
    }
    if (pawn_attacks_span(pos, square, NULL))
    {
        return 0;
    }
    return 1;
}

int reachable_outpost(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, reachable_outpost, NULL);
    }
    char piece = board(pos, square->x, square->y);
    if (piece != 'B' && piece != 'N')
    {
        return 0;
    }
    int v = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 2; y < 5; y++)
        {
            Square s2 = {x, y};
            if ((piece == 'N' && strchr("PNBRQK", board(pos, x, y)) == NULL &&
                 knight_attack(pos, &s2, square) && outpost_square(pos, &s2, NULL)) ||
                (piece == 'B' && strchr("PNBRQK", board(pos, x, y)) == NULL &&
                 bishop_xray_attack(pos, &s2, square) && outpost_square(pos, &s2, NULL)))
            {
                int support = (board(pos, x - 1, y + 1) == 'P' || board(pos, x + 1, y + 1) == 'P') ? 2 : 1;
                v = MAX(v, support);
            }
        }
    }
    return v;
}

int minor_behind_pawn(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, minor_behind_pawn, NULL);
    }
    char piece = board(pos, square->x, square->y);
    if (piece != 'B' && piece != 'N')
    {
        return 0;
    }
    if (toupper(board(pos, square->x, square->y - 1)) != 'P')
    {
        return 0;
    }
    return 1;
}

int bishop_pawns(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, bishop_pawns, NULL);
    }
    if (board(pos, square->x, square->y) != 'B')
    {
        return 0;
    }
    int c = (square->x + square->y) % 2;
    int v = 0, blocked = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'P' && c == (x + y) % 2)
            {
                v++;
            }
            if (board(pos, x, y) == 'P' && x > 1 && x < 6 && board(pos, x, y - 1) != '-')
            {
                blocked++;
            }
        }
    }
    return v * (blocked + (pawn_attack(pos, square, NULL) > 0 ? 0 : 1));
}

int rook_on_file(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, rook_on_file, NULL);
    }
    if (board(pos, square->x, square->y) != 'R')
    {
        return 0;
    }
    int open = 1;
    for (int y = 0; y < 8; y++)
    {
        if (board(pos, square->x, y) == 'P')
        {
            return 0;
        }
        if (board(pos, square->x, y) == 'p')
        {
            open = 0;
        }
    }
    return open + 1;
}

int trapped_rook(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, trapped_rook, NULL);
    }
    if (board(pos, square->x, square->y) != 'R')
    {
        return 0;
    }
    if (rook_on_file(pos, square, NULL))
    {
        return 0;
    }
    if (mobility(pos, square, NULL) > 3)
    {
        return 0;
    }
    int kx = 0, ky = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'K')
            {
                kx = x;
                ky = y;
            }
        }
    }
    if ((kx < 4) != (square->x < kx))
    {
        return 0;
    }
    return 1;
}

int weak_queen(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, weak_queen, NULL);
    }
    if (board(pos, square->x, square->y) != 'Q')
    {
        return 0;
    }
    for (int i = 0; i < 8; i++)
    {
        int ix = (i + (i > 3)) % 3 - 1;
        int iy = ((i + (i > 3)) / 3) - 1;
        int count = 0;
        for (int d = 1; d < 8; d++)
        {
            char b = board(pos, square->x + d * ix, square->y + d * iy);
            if ((b == 'r' && (ix == 0 || iy == 0) && count == 1) ||
                (b == 'b' && (ix != 0 && iy != 0) && count == 1))
            {
                return 1;
            }
            if (b != '-')
            {
                count++;
            }
        }
    }
    return 0;
}

int king_protector(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, king_protector, NULL);
    }
    char piece = board(pos, square->x, square->y);
    if (piece != 'N' && piece != 'B')
    {
        return 0;
    }
    return king_distance(pos, square, NULL);
}

int long_diagonal_bishop(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, long_diagonal_bishop, NULL);
    }
    if (board(pos, square->x, square->y) != 'B')
    {
        return 0;
    }
    if (square->x - square->y != 0 && square->x - (7 - square->y) != 0)
    {
        return 0;
    }
    int x1 = square->x, y1 = square->y;
    if (MIN(x1, 7 - x1) > 2)
    {
        return 0;
    }
    for (int i = MIN(x1, 7 - x1); i < 4; i++)
    {
        if (board(pos, x1, y1) == 'p' || board(pos, x1, y1) == 'P')
        {
            return 0;
        }
        x1 = x1 < 4 ? x1 + 1 : x1 - 1;
        y1 = y1 < 4 ? y1 + 1 : y1 - 1;
    }
    return 1;
}

int outpost_total(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, outpost_total, NULL);
    }
    char piece = board(pos, square->x, square->y);
    if (piece != 'N' && piece != 'B')
    {
        return 0;
    }
    int knight = (piece == 'N');
    int reachable = 0;
    if (!outpost(pos, square, NULL))
    {
        if (!knight)
        {
            return 0;
        }
        reachable = reachable_outpost(pos, square, NULL);
        if (!reachable)
        {
            return 0;
        }
        return 1;
    }
    if (knight && (square->x < 2 || square->x > 5))
    {
        int ea = 0, cnt = 0;
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                if ((abs(square->x - x) == 2 && abs(square->y - y) == 1) ||
                    (abs(square->x - x) == 1 && abs(square->y - y) == 2))
                {
                    if (strchr("nbrqk", board(pos, x, y)) != NULL)
                    {
                        ea = 1;
                    }
                }
                if ((x < 4 && square->x < 4) || (x >= 4 && square->x >= 4))
                {
                    if (strchr("nbrqk", board(pos, x, y)) != NULL)
                    {
                        cnt++;
                    }
                }
            }
        }
        if (!ea && cnt <= 1)
        {
            return 2;
        }
    }
    return knight ? 4 : 3;
}

int rook_on_queen_file(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, rook_on_queen_file, NULL);
    }
    if (board(pos, square->x, square->y) != 'R')
    {
        return 0;
    }
    for (int y = 0; y < 8; y++)
    {
        if (toupper(board(pos, square->x, y)) == 'Q')
        {
            return 1;
        }
    }
    return 0;
}

int bishop_xray_pawns(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, bishop_xray_pawns, NULL);
    }
    if (board(pos, square->x, square->y) != 'B')
    {
        return 0;
    }
    int count = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'p' &&
                abs(square->x - x) == abs(square->y - y))
            {
                count++;
            }
        }
    }
    return count;
}

int rook_on_king_ring(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, rook_on_king_ring, NULL);
    }
    if (board(pos, square->x, square->y) != 'R')
    {
        return 0;
    }
    if (king_attackers_count(pos, square, NULL) > 0)
    {
        return 0;
    }
    for (int y = 0; y < 8; y++)
    {
        if (king_ring(pos, &(Square){square->x, y}, &(int){0}))
        {
            return 1;
        }
    }

    return 0;
}

int queen_infiltration(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, queen_infiltration, NULL);
    }
    if (board(pos, square->x, square->y) != 'Q')
    {
        return 0;
    }
    if (square->y > 3)
    {
        return 0;
    }
    if (board(pos, square->x + 1, square->y - 1) == 'p' ||
        board(pos, square->x - 1, square->y - 1) == 'p')
    {
        return 0;
    }
    if (pawn_attacks_span(pos, square, NULL))
    {
        return 0;
    }
    return 1;
}

int pieces_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, pieces_mg, NULL);
    }
    if (strchr("NBRQ", board(pos, square->x, square->y)) == NULL)
    {
        return 0;
    }
    int v = 0;
    v += (int[]){0, 31, -7, 30, 56}[outpost_total(pos, square, NULL)];
    v += 18 * minor_behind_pawn(pos, square, NULL);
    v -= 3 * bishop_pawns(pos, square, NULL);
    v -= 4 * bishop_xray_pawns(pos, square, NULL);
    v += 6 * rook_on_queen_file(pos, square, NULL);
    v += 16 * rook_on_king_ring(pos, square, NULL);
    v += 24 * bishop_on_king_ring(pos, square, NULL);
    v += (int[]){0, 19, 48}[rook_on_file(pos, square, NULL)];
    v -= trapped_rook(pos, square, NULL) * 55 * (pos->c[0] || pos->c[1] ? 1 : 2);
    v -= 56 * weak_queen(pos, square, NULL);
    v -= 2 * queen_infiltration(pos, square, NULL);
    v -= (board(pos, square->x, square->y) == 'N' ? 8 : 6) * king_protector(pos, square, NULL);
    v += 45 * long_diagonal_bishop(pos, square, NULL);
    return v;
}

int pieces_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, pieces_eg, NULL);
    }
    if (strchr("NBRQ", board(pos, square->x, square->y)) == NULL)
    {
        return 0;
    }
    int v = 0;
    v += (int[]){0, 22, 36, 23, 36}[outpost_total(pos, square, NULL)];
    v += 3 * minor_behind_pawn(pos, square, NULL);
    v -= 7 * bishop_pawns(pos, square, NULL);
    v -= 5 * bishop_xray_pawns(pos, square, NULL);
    v += 11 * rook_on_queen_file(pos, square, NULL);
    v += (int[]){0, 7, 29}[rook_on_file(pos, square, NULL)];
    v -= trapped_rook(pos, square, NULL) * 13 * (pos->c[0] || pos->c[1] ? 1 : 2);
    v -= 15 * weak_queen(pos, square, NULL);
    v += 14 * queen_infiltration(pos, square, NULL);
    v -= 9 * king_protector(pos, square, NULL);
    return v;
}

int bishop_on_king_ring(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, bishop_on_king_ring, NULL);
    }
    if (board(pos, square->x, square->y) != 'B')
    {
        return 0;
    }
    if (king_attackers_count(pos, square, NULL) > 0)
    {
        return 0;
    }
    for (int i = 0; i < 4; i++)
    {
        int ix = ((i > 1) * 2 - 1);
        int iy = ((i % 2 == 0) * 2 - 1);
        for (int d = 1; d < 8; d++)
        {
            int x = square->x + d * ix;
            int y = square->y + d * iy;
            if (x < 0 || x >= 8 || y < 0 || y >= 8)
            {
                break;
            }
            if (board(pos, x, y) == 'x')
            {
                break;
            }
            if (king_ring(pos, &(Square){x, y}, &(int){0}))
            {
                return 1;
            }
            if (toupper(board(pos, x, y)) == 'P')
            {
                break;
            }
        }
    }

    return 0;
}