#include "header/chess/material.h"

int non_pawn_material(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square == NULL)
    {
        return sum(pos, non_pawn_material, NULL);
    }

    char piece = board(pos, square->x, square->y);
    // Check if the piece is one of "NBRQ" (Knight, Bishop, Rook, Queen)
    if (piece == 'N' || piece == 'B' || piece == 'R' || piece == 'Q')
    {
        return piece_value_bonus(pos, square, &(bool){true});
    }

    return 0; // Return 0 if it's not one of "NBRQ"
}

int piece_value_bonus(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    bool mg = *(bool *)param;
    if (!square)
        return sum(pos, piece_value_bonus, param);

    static const int mg_values[] = {124, 781, 825, 1276, 2538};
    static const int eg_values[] = {206, 854, 915, 1380, 2682};

    const int *a = mg ? mg_values : eg_values; // Pointer to the correct array
    char piece = board(pos, square->x, square->y);

    char *found = strchr("PNBRQ", piece); // Check if the piece is in "PNBRQ"
    if (found == NULL)
    {
        return 0;
    }

    int i = found - "PNBRQ"; // Calculate the index

    return a[i]; // Access the array element based on `i`
}

int psqt_bonus(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    bool mg = *(bool *)param;

    if (!square)
        return sum(pos, psqt_bonus, param);

    int bonus[5][8][4] = {
        {{-175, -92, -74, -73}, {-77, -41, -27, -15}, {-61, -17, 6, 12}, {-35, 8, 40, 49}, {-34, 13, 44, 51}, {-9, 22, 58, 53}, {-67, -27, 4, 37}, {-201, -83, -56, -26}},
        {{-53, -5, -8, -23}, {-15, 8, 19, 4}, {-7, 21, -5, 17}, {-5, 11, 25, 39}, {-12, 29, 22, 31}, {-16, 6, 1, 11}, {-17, -14, 5, 0}, {-48, 1, -14, -23}},
        {{-31, -20, -14, -5}, {-21, -13, -8, 6}, {-25, -11, -1, 3}, {-13, -5, -4, -6}, {-27, -15, -4, 3}, {-22, -2, 6, 12}, {-2, 12, 16, 18}, {-17, -19, -1, 9}},
        {{3, -5, -5, 4}, {-3, 5, 8, 12}, {-3, 6, 13, 7}, {4, 5, 9, 8}, {0, 14, 12, 5}, {-4, 10, 6, 8}, {-5, 6, 10, 8}, {-2, -2, 1, -2}},
        {{271, 327, 271, 198}, {278, 303, 234, 179}, {195, 258, 169, 120}, {164, 190, 138, 98}, {154, 179, 105, 70}, {123, 145, 81, 31}, {88, 120, 65, 33}, {59, 89, 45, -1}}};

    int pbonus[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0}, {3, 3, 10, 19, 16, 19, 7, -5}, {-9, -15, 11, 15, 32, 22, 5, -22}, {-4, -23, 6, 20, 40, 17, 4, -8}, {13, 0, -13, 1, 11, -2, -13, 5}, {5, -12, -7, 22, -8, -5, -15, -8}, {-7, 7, -3, -13, 5, -16, 10, -8}, {0, 0, 0, 0, 0, 0, 0, 0}};

    if (square->x < 0 || square->x >= 8 || square->y < 0 || square->y >= 8)
    {
        return 0;
    }

    char piece = board(pos, square->x, square->y);

    int i = strchr("PNBRQK", piece) - "PNBRQK";
    if (i < 0 || i >= 6)
    {
        return 0;
    }
    if (i == 0)
    {
        return pbonus[7 - square->y][square->x];
    }
    else
    {
        int mirrored_file = MIN(square->x, 7 - square->x);
        return bonus[i - 1][7 - square->y][mirrored_file];
    }
}

int piece_value_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (!square)
        return sum(pos, piece_value_mg, param);
    return piece_value_bonus(pos, square, &(bool){true});
}

int piece_value_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (!square)
        return sum(pos, piece_value_eg, param);
    return piece_value_bonus(pos, square, &(bool){false});
}

int psqt_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (!square)
        return sum(pos, psqt_mg, NULL);
    return psqt_bonus(pos, square, &(bool){true});
}

int psqt_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (!square)
        return sum(pos, psqt_eg, NULL);
    return psqt_bonus(pos, square, &(bool){false});
}
