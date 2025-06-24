#include "header/chess/mobility.h"
int mobility_area(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = pos1;
    if (!square)
        return sum(pos, mobility_area, NULL);
    if (board(pos, square->x, square->y) == 'K')
        return 0;
    if (board(pos, square->x, square->y) == 'Q')
        return 0;
    if (board(pos, square->x - 1, square->y - 1) == 'p')
        return 0;
    if (board(pos, square->x + 1, square->y - 1) == 'p')
        return 0;
    if (board(pos, square->x, square->y) == 'P' &&
        (rank(pos, square, NULL) < 4 || board(pos, square->x, square->y - 1) != '-'))
        return 0;
    if (blockers_for_king(pos2, &(Square){square->x, 7 - square->y}, NULL))
        return 0;
    return 1;
}

int mobility(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (!square)
        return sum(pos, mobility, NULL);
    int v = 0;
    char b = board(pos, square->x, square->y);
    if (strchr("NBRQ", b) == NULL)
        return 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            Square s2 = {x, y};
            if (!mobility_area(pos, &s2, NULL))
                continue;
            if (b == 'N' && knight_attack(pos, &s2, square) && board(pos, x, y) != 'Q')
                v++;
            if (b == 'B' && bishop_xray_attack(pos, &s2, square) && board(pos, x, y) != 'Q')
                v++;
            if (b == 'R' && rook_xray_attack(pos, &s2, square))
                v++;
            if (b == 'Q' && queen_attack(pos, &s2, square))
                v++;
        }
    }

    return v;
}

int mobility_bonus(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    // Ensure param is defined and cast to bool*
    bool mg = *(bool *)param;

    if (!square)
        return sum(pos, mobility_bonus, param);
    if (!square)
        return sum(pos, mobility_bonus, param); // Pass param directly
    static const int bonus_mg[4][29] = {
        {-62, -53, -12, -4, 3, 13, 22, 28, 33},
        {-48, -20, 16, 26, 38, 51, 55, 63, 63, 68, 81, 81, 91, 98},
        {-60, -20, 2, 3, 3, 11, 22, 31, 40, 40, 41, 48, 57, 57, 62},
        {-30, -12, -8, -9, 20, 23, 23, 35, 38, 53, 64, 65, 65, 66, 67, 67, 72, 72, 77, 79, 93, 108, 108, 108, 110, 114, 114, 116}};
    static const int bonus_eg[4][29] = {
        {-81, -56, -31, -16, 5, 11, 17, 20, 25},
        {-59, -23, -3, 13, 24, 42, 54, 57, 65, 73, 78, 86, 88, 97},
        {-78, -17, 23, 39, 70, 99, 103, 121, 134, 139, 158, 164, 168, 169, 172},
        {-48, -30, -7, 19, 40, 55, 59, 75, 78, 96, 96, 100, 121, 127, 131, 133, 136, 141, 147, 150, 151, 168, 168, 171, 182, 182, 192, 219}};
    const int (*bonus)[29] = mg ? bonus_mg : bonus_eg;

    char b = board(pos, square->x, square->y);
    int i = strchr("NBRQ", b) - "NBRQ";
    if (i < 0 || i >= 4)
        return 0;
    return bonus[i][mobility(pos, square, NULL)];
}

int mobility_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
        return sum(pos, mobility_mg, NULL);
    return mobility_bonus(pos, square, &(bool){true});
}

int mobility_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (!square)
        return sum(pos, mobility_eg, NULL);
    return mobility_bonus(pos, square, &(bool){false});
}