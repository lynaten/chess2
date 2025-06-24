#include "header/chess/imbalance.h"
int imbalance(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (!square)
        return sum(pos, imbalance, NULL);

    // Arrays for imbalance bonuses
    int qo[6][6] = {
        {0}, {40, 38}, {32, 255, -62}, {0, 104, 4, 0}, {-26, -2, 47, 105, -208}, {-189, 24, 117, 133, -134, -6}};
    int qt[6][6] = {
        {0}, {36, 0}, {9, 63, 0}, {59, 65, 42, 0}, {46, 39, 24, -24, 0}, {97, 100, -42, 137, 268, 0}};

    // Get the piece on the given square
    char piece = board(pos, square->x, square->y);
    int j = strchr("XPNBRQxpnbrq", piece) - "XPNBRQxpnbrq";

    // Ensure the index is valid
    if (j < 0 || j >= 6)
        return 0;

    int bishop[2] = {0, 0};
    int v = 0;

    // Loop through all squares on the board
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            char p = board(pos, x, y);
            int i = strchr("XPNBRQxpnbrq", p) - "XPNBRQxpnbrq";

            // Ensure the index is valid
            if (i < 0 || i >= 12)
                continue;

            // Count bishops for special handling
            if (i == 9) // Black bishop
                bishop[0]++;
            if (i == 3) // White bishop
                bishop[1]++;

            // Skip pieces that cannot interact with `j`
            if (i % 6 > j)
                continue;

            // Add bonuses for interactions
            if (i >= 6) // Black pieces interacting with white piece
                v += qt[j][i - 6];
            else // White pieces interacting with white piece
                v += qo[j][i];
        }
    }

    // Add bonuses for multiple bishops
    if (bishop[0] > 1)
        v += qt[j][0];
    if (bishop[1] > 1)
        v += qo[j][0];

    return v;
}

int bishop_pair(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (bishop_count(pos, NULL, NULL) < 2)
        return 0;

    if (!square)
        return 1438;

    return board(pos, square->x, square->y) == 'B' ? 1 : 0;
}

int imbalance_total(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped)
{
    ChessPosition *pos = pos1;
    int v = 0;
    v += imbalance(pos, NULL, NULL) - imbalance(pos2, NULL, NULL);
    v += bishop_pair(pos, NULL, NULL) - bishop_pair(pos2, NULL, NULL);

    return v / 16;
}
