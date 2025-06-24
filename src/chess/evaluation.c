#include "header/chess/evaluation.h"

int main_evaluation(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    int mg = middle_game_evaluation(pos, false);
    int eg = end_game_evaluation(pos, false);
    int p = phase(pos);
    int rule50 = rule(pos, NULL);
    float scale_factor_val = scale_factor(pos, eg);
    float scaled_eg = (float)eg * scale_factor_val / 64;
    int v = (int)(((mg * p + (scaled_eg * (128 - p))) / 128));
    v = ((v / 16) * 16);
    int tempo_val = tempo(pos, NULL);
    v += tempo_val;
    v = (v * (100 - rule50) / 100);
    return v;
}

int middle_game_evaluation(ChessPosition *pos1, ChessPosition *pos2, bool nowinnable, bool colorflipped)
{
    ChessPosition *pos = pos1;
    int v = 0;
    int white_piece_value = piece_value_mg(pos, NULL, NULL);
    int black_piece_value = piece_value_mg(pos2, NULL, NULL);
    v += white_piece_value - black_piece_value;
    int white_psqt = psqt_mg(pos, NULL, NULL);
    int black_psqt = psqt_mg(pos2, NULL, NULL);
    v += white_psqt - black_psqt;
    int imbalance = imbalance_total(pos);
    v += imbalance;
    int white_pawns = pawns_mg(pos, NULL, NULL);
    int black_pawns = pawns_mg(pos2, NULL, NULL);
    v += white_pawns - black_pawns;
    int white_pieces = pieces_mg(pos, NULL, NULL);
    int black_pieces = pieces_mg(pos2, NULL, NULL);
    v += white_pieces - black_pieces;
    int white_mobility = mobility_mg(pos, NULL, NULL);
    int black_mobility = mobility_mg(pos2, NULL, NULL);
    v += white_mobility - black_mobility;
    int white_threats = threats_mg(pos);
    int black_threats = threats_mg(pos2);
    v += white_threats - black_threats; // Assuming `threats_mg` is a combined score for that side.
    int white_passed = passed_mg(pos, NULL, NULL);
    int black_passed = passed_mg(pos2, NULL, NULL);
    v += white_passed - black_passed;
    int white_space = space(pos);
    int black_space = space(pos2);
    v += white_space - black_space;
    int white_king = king_mg(pos);
    int black_king = king_mg(pos2);
    v += white_king - black_king;
    if (!nowinnable)
    {
        int winnable = winnable_total_mg(pos, &(int){v});
        v += winnable;
    }
    return v;
}

int end_game_evaluation(ChessPosition *pos1, ChessPosition *pos2, bool nowinnable, bool colorflipped)
{
    ChessPosition *pos = pos1;
    int v = 0;
    int piece_value_white = piece_value_eg(pos, NULL, NULL);
    int piece_value_black = piece_value_eg(pos2, NULL, NULL);
    v += piece_value_white - piece_value_black;
    int psqt_white = psqt_eg(pos, NULL, NULL);
    int psqt_black = psqt_eg(pos2, NULL, NULL);
    v += psqt_white - psqt_black;
    int imbalance_value = imbalance_total(pos);
    v += imbalance_value;
    int pawns_white = pawns_eg(pos, NULL, NULL);
    int pawns_black = pawns_eg(pos2, NULL, NULL);
    v += pawns_white - pawns_black;
    int pieces_white = pieces_eg(pos, NULL, NULL);
    int pieces_black = pieces_eg(pos2, NULL, NULL);
    v += pieces_white - pieces_black;
    int mobility_white = mobility_eg(pos, NULL, NULL);
    int mobility_black = mobility_eg(pos2, NULL, NULL);
    v += mobility_white - mobility_black;
    int threats_white = threats_eg(pos);
    int threats_black = threats_eg(pos2);
    v += threats_white - threats_black;
    int passed_white = passed_eg(pos, NULL, NULL);
    int passed_black = passed_eg(pos2, NULL, NULL);
    v += passed_white - passed_black;
    int king_white = king_eg(pos);
    int king_black = king_eg(pos2);
    v += king_white - king_black;
    if (!nowinnable)
    {
        int winnable_value = winnable_total_eg(pos, &(int){v});
        v += winnable_value;
    }

    return v;
}

int scale_factor(ChessPosition *pos1, ChessPosition *pos2, int eg, bool colorflipped)
{
    ChessPosition *pos = pos1;
    ChessPosition *pos_w = (eg > 0) ? pos : pos2;
    ChessPosition *pos_b = (eg > 0) ? pos2 : pos;
    int sf = 64;
    int pc_w = pawn_count(pos_w, NULL, NULL), pc_b = pawn_count(pos_b, NULL, NULL);
    int qc_w = queen_count(pos_w, NULL, NULL), qc_b = queen_count(pos_b, NULL, NULL);
    int bc_w = bishop_count(pos_w, NULL, NULL), bc_b = bishop_count(pos_b, NULL, NULL);
    int nc_w = knight_count(pos_w, NULL, NULL), nc_b = knight_count(pos_b, NULL, NULL);
    int npm_w = non_pawn_material(pos_w, NULL, NULL), npm_b = non_pawn_material(pos_b, NULL, NULL);

    const int bishopValueMg = 825, bishopValueEg = 915, rookValueMg = 1276;

    // Case 1: No pawns for white and limited material
    if (pc_w == 0 && npm_w - npm_b <= bishopValueMg)
    {
        sf = (npm_w < rookValueMg) ? 0 : (npm_b <= bishopValueMg) ? 4
                                                                  : 14;
    }

    if (sf == 64)
    {
        // Opposite bishops
        int ob = opposite_bishops(pos);
        if (ob && npm_w == bishopValueMg && npm_b == bishopValueMg)
        {
            sf = 22 + 4 * candidate_passed(pos_w, NULL, NULL);
        }
        else if (ob)
        {
            sf = 22 + 3 * piece_count(pos_w, NULL, NULL);
        }
        else
        {
            // Both sides have one rook each and pawns
            if (npm_w == rookValueMg && npm_b == rookValueMg && pc_w - pc_b <= 1)
            {
                int pawnking_b = 0, pcw_flank[2] = {0, 0};

                for (int x = 0; x < 8; x++)
                {
                    for (int y = 0; y < 8; y++)
                    {
                        char piece = board(pos_w, x, y);
                        if (piece == 'P')
                        {
                            pcw_flank[(x < 4) ? 0 : 1] = 1;
                        }

                        piece = board(pos_b, x, y);
                        if (piece == 'K')
                        {
                            for (int ix = -1; ix <= 1; ix++)
                            {
                                for (int iy = -1; iy <= 1; iy++)
                                {
                                    if (board(pos_b, x + ix, y + iy) == 'P')
                                    {
                                        pawnking_b = 1;
                                    }
                                }
                            }
                        }
                    }
                }

                if (pcw_flank[0] != pcw_flank[1] && pawnking_b)
                {
                    return 36;
                }
            }

            // One queen present on either side
            if (qc_w + qc_b == 1)
            {
                sf = 37 + 3 * ((qc_w == 1) ? (bc_b + nc_b) : (bc_w + nc_w));
            }
            else
            {
                sf = MIN(sf, 36 + 7 * pc_w);
            }
        }
    }

    return sf;
}

int phase(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped)
{
    ChessPosition *pos = pos1;
    const int midgameLimit = 15258;
    const int endgameLimit = 3915;

    int npm = non_pawn_material(pos, NULL, NULL) + non_pawn_material(pos2, NULL, NULL);

    npm = fmax(endgameLimit, fmin(npm, midgameLimit));

    return ((npm - endgameLimit) * 128) /
           (midgameLimit - endgameLimit);
}

int tempo(ChessPosition *pos1, ChessPosition *pos2, Square *square, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square != NULL)
    {
        return 0;
    }
    return 28 * (pos->w ? 1 : -1);
}
int rule(ChessPosition *pos1, ChessPosition *pos2, Square *square, bool colorflipped)
{
    ChessPosition *pos = colorflipped ? pos2 : pos1;
    if (square != NULL)
    {
        return 0;
    }
    return pos->m[0];
}