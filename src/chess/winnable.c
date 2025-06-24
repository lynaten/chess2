#include "header/chess/winnable.h"

int winnable(ChessPosition *pos)
{
    int pawns = 0, kx[2] = {0, 0}, ky[2] = {0, 0}, flanks[2] = {0, 0};

    for (int x = 0; x < 8; x++)
    {
        int open[2] = {0, 0};
        for (int y = 0; y < 8; y++)
        {
            char piece = board(pos, x, y);

            if (toupper(piece) == 'P')
            {
                open[piece == 'P' ? 0 : 1] = 1;
                pawns++;
            }

            if (toupper(piece) == 'K')
            {
                kx[piece == 'K' ? 0 : 1] = x;
                ky[piece == 'K' ? 0 : 1] = y;
            }
        }

        if (open[0] + open[1] > 0)
        {
            flanks[x < 4 ? 0 : 1] = 1;
        }
    }

    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    int passedCount = candidate_passed(pos, NULL, NULL) + candidate_passed(pos2, NULL, NULL);
    int bothFlanks = (flanks[0] && flanks[1]) ? 1 : 0;
    int outflanking = abs(kx[0] - kx[1]) - abs(ky[0] - ky[1]);
    int purePawn = (non_pawn_material(pos, NULL, NULL) + non_pawn_material(pos2, NULL, NULL)) == 0 ? 1 : 0;
    int almostUnwinnable = (outflanking < 0 && bothFlanks == 0);
    int infiltration = (ky[0] < 4 || ky[1] > 3) ? 1 : 0;

    return 9 * passedCount + 12 * pawns + 9 * outflanking + 21 * bothFlanks + 24 * infiltration + 51 * purePawn - 43 * almostUnwinnable - 110;
}

int winnable_total_mg(ChessPosition *pos, void *param)
{
    int v = *(int *)param;

    if (param == NULL)
    {
        v = middle_game_evaluation(pos, true);
    }

    // Calculate winnable_value
    int winnable_value = winnable(pos) + 50;


    // Calculate bounded_value
    int bounded_value = (winnable_value > 0) ? 0 : -abs(v);

    // Calculate final result
    int sign = (v > 0) ? 1 : (v < 0 ? -1 : 0);

    int result = sign * fmax(bounded_value, -abs(v));

    return result;
}

int winnable_total_eg(ChessPosition *pos, void *param)
{
    int v = *(int *)param;

    if (param == NULL)
    {
        v = end_game_evaluation(pos, true);
    }

    int winnable_value = winnable(pos);
    int bounded_value = -abs(v);

    return (v > 0 ? 1 : (v < 0 ? -1 : 0)) * fmax(winnable_value, bounded_value);
}
