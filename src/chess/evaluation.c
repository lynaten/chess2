#include "header/chess/evaluation.h"
#include <time.h>

// #include <time.h>

int main_evaluation(ChessPosition *pos)
{
    // clock_t start_total = clock();

    // Step 1: Middle-game evaluation
    // clock_t start_mg = clock();
    int mg = middle_game_evaluation(pos, false);
    // clock_t end_mg = clock();
    // double elapsed_mg = 1000.0 * (end_mg - start_mg) / CLOCKS_PER_SEC;

    // Step 2: End-game evaluation
    // clock_t start_eg = clock();
    int eg = end_game_evaluation(pos, false);
    // clock_t end_eg = clock();
    // double elapsed_eg = 1000.0 * (end_eg - start_eg) / CLOCKS_PER_SEC;

    // Step 3: Phase
    // clock_t start_phase = clock();
    int p = phase(pos);
    // clock_t end_phase = clock();
    // double elapsed_phase = 1000.0 * (end_phase - start_phase) / CLOCKS_PER_SEC;

    // Step 4: Rule 50
    // clock_t start_rule = clock();
    int rule50 = rule(pos, NULL);
    // clock_t end_rule = clock();
    // double elapsed_rule = 1000.0 * (end_rule - start_rule) / CLOCKS_PER_SEC;

    // Step 5: Scale factor
    // clock_t start_sf = clock();
    float scale_factor_val = scale_factor(pos, eg);
    // clock_t end_sf = clock();
    // double elapsed_sf = 1000.0 * (end_sf - start_sf) / CLOCKS_PER_SEC;

    // Step 6: Combine evaluations
    float scaled_eg = (float)eg * scale_factor_val / 64;
    int v = (int)(((mg * p + (scaled_eg * (128 - p))) / 128));

    v = ((v / 16) * 16);

    // Step 8: Tempo
    // clock_t start_tempo = clock();
    int tempo_val = tempo(pos, NULL);
    // clock_t end_tempo = clock();
    // double elapsed_tempo = 1000.0 * (end_tempo - start_tempo) / CLOCKS_PER_SEC;

    v += tempo_val;

    // Step 9: Rule 50 reduction
    v = (v * (100 - rule50) / 100);

    // clock_t end_total = clock();
    // double elapsed_total = 1000.0 * (end_total - start_total) / CLOCKS_PER_SEC;

    // Output debug timing:
    // printf("[Eval Timing] mg: %.3f ms | eg: %.3f ms | phase: %.3f ms | rule: %.3f ms | scale: %.3f ms | tempo: %.3f ms | total: %.3f ms\n",
    //        elapsed_mg, elapsed_eg, elapsed_phase, elapsed_rule, elapsed_sf, elapsed_tempo, elapsed_total);

    return v;
}

int main1_evaluation(ChessPosition *pos)
{
}

int middle_game_evaluation(ChessPosition *pos, bool nowinnable)
{
    clock_t start_total = clock(); // Start timer for total evaluation time

    int v = 0;
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;

    clock_t start_segment = clock();
    colorflip(pos, pos2);
    double elapsed_colorflip = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] colorflip: %.3f ms\n", elapsed_colorflip);


    // Piece value
    start_segment = clock();
    int white_piece_value = piece_value_mg(pos, NULL, NULL);
    int black_piece_value = piece_value_mg(pos2, NULL, NULL);
    pos->eval.piece_value = white_piece_value - black_piece_value;
    pos2->eval.piece_value = black_piece_value - white_piece_value;
    v += white_piece_value - black_piece_value;
    double elapsed_piece_value = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] piece_value_mg: %.3f ms\n", elapsed_piece_value);


    // Piece-square table
    start_segment = clock();
    int white_psqt = psqt_mg(pos, NULL, NULL);
    int black_psqt = psqt_mg(pos2, NULL, NULL);
    pos->eval.psqt = white_psqt - black_psqt;
    pos2->eval.psqt = black_psqt - white_psqt;
    v += white_psqt - black_psqt;
    double elapsed_psqt = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] psqt_mg: %.3f ms\n", elapsed_psqt);


    // Imbalance
    start_segment = clock();
    int imbalance = imbalance_total(pos);
    // Note: If you also calculate imbalance for pos2, add it here.
    // For now, based on your code, it's a single calculation.
    pos->eval.imbalance = imbalance; // Assuming you have this field in EvalData
    v += imbalance;
    double elapsed_imbalance = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] imbalance_total: %.3f ms\n", elapsed_imbalance);


    // Pawn structure
    start_segment = clock();
    // Re-ensure pawns_list is initialized inside pawn_file_count or explicitly here
    // pawn_file_count(pos, NULL, NULL); // If this populates pawns_list for pawns_mg
    int white_pawns = pawns_mg(pos, NULL, NULL);
    int black_pawns = pawns_mg(pos2, NULL, NULL);
    pos->eval.pawns = white_pawns - black_pawns;
    pos2->eval.pawns = black_pawns - white_pawns;
    v += white_pawns - black_pawns;
    double elapsed_pawns = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] pawns_mg: %.3f ms\n", elapsed_pawns);


    // Pieces (material counts like bishop_pair, knight_defender, queen_count, etc.)
    start_segment = clock();
    int white_pieces = pieces_mg(pos, NULL, NULL);
    int black_pieces = pieces_mg(pos2, NULL, NULL);
    pos->eval.pieces = white_pieces - black_pieces;
    pos2->eval.pieces = black_pieces - white_pieces;
    // Assuming pieces_mg covers bishop_pair, queen_count, knight_defender, etc.
    // If not, call them separately and time them.
    v += white_pieces - black_pieces;
    double elapsed_pieces = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] pieces_mg: %.3f ms\n", elapsed_pieces);


    // Mobility
    start_segment = clock();
    int white_mobility = mobility_mg(pos, NULL, NULL);
    int black_mobility = mobility_mg(pos2, NULL, NULL);
    pos->eval.mobility_white = white_mobility;
    pos->eval.mobility_black = black_mobility;
    pos2->eval.mobility_white = black_mobility;
    pos2->eval.mobility_black = white_mobility;
    v += white_mobility - black_mobility;
    double elapsed_mobility = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] mobility_mg: %.3f ms\n", elapsed_mobility);


    // Threats (includes king_threat, pawn_push_threat, root_threat, flank_attack, flank_defense etc.)
    start_segment = clock();
    int white_threats = threats_mg(pos);
    int black_threats = threats_mg(pos2);
    // Assuming threats_mg populates sub-fields like pos->eval.king_threat etc.
    // If not, you'd call/time each specific threat function here.
    pos->eval.root_threat = white_threats; // Example of storing white's raw threats
    // pos2->eval.root_threat = black_threats; // If you want to mirror it for pos2
    v += white_threats - black_threats; // Assuming `threats_mg` is a combined score for that side.
    double elapsed_threats = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] threats_mg: %.3f ms\n", elapsed_threats);


    // Passed pawns
    start_segment = clock();
    int white_passed = passed_mg(pos, NULL, NULL);
    int black_passed = passed_mg(pos2, NULL, NULL);
    pos->eval.passed = white_passed - black_passed;
    pos2->eval.passed = black_passed - white_passed;
    v += white_passed - black_passed;
    double elapsed_passed = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] passed_mg: %.3f ms\n", elapsed_passed);


    // Space
    start_segment = clock();
    int white_space = space(pos);
    int black_space = space(pos2);
    pos->eval.space_area = white_space - black_space; // Assuming space_area is the diff
    // If you want raw scores for space: pos->eval.space_white_raw = white_space; pos->eval.space_black_raw = black_space;
    v += white_space - black_space;
    double elapsed_space = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] space: %.3f ms\n", elapsed_space);


    // King safety
    start_segment = clock();
    int white_king = king_mg(pos);
    int black_king = king_mg(pos2);
    pos->eval.blockers_for_king = white_king - black_king; // Assuming this is the diff score
    // If you want raw scores: pos->eval.king_white_raw = white_king; pos->eval.king_black_raw = black_king;
    v += white_king - black_king;
    double elapsed_king_safety = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] king_mg: %.3f ms\n", elapsed_king_safety);


    // Winnable positions
    start_segment = clock();
    if (!nowinnable)
    {
        int winnable = winnable_total_mg(pos, &(int){v});
        v += winnable;
    }
    double elapsed_winnable = (double)(clock() - start_segment) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] winnable_total_mg: %.3f ms\n", elapsed_winnable);


    double total_eval_time = (double)(clock() - start_total) * 1000.0 / CLOCKS_PER_SEC;
    printf("[DEBUG-TIME] Total middle_game_evaluation: %.3f ms\n", total_eval_time);

    return v;
}

int end_game_evaluation(ChessPosition *pos, bool nowinnable)
{
    int v = 0;
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);

    // Step 1: Piece Value Evaluation
    int piece_value_white = piece_value_eg(pos, NULL, NULL);
    int piece_value_black = piece_value_eg(pos2, NULL, NULL);
    v += piece_value_white - piece_value_black;

    // Step 2: Piece-Square Table (PSQT) Evaluation
    int psqt_white = psqt_eg(pos, NULL, NULL);
    int psqt_black = psqt_eg(pos2, NULL, NULL);
    v += psqt_white - psqt_black;

    // Step 3: Imbalance Evaluation
    int imbalance_value = imbalance_total(pos);
    v += imbalance_value;

    // Step 4: Pawn Structure Evaluation
    int pawns_white = pawns_eg(pos, NULL, NULL);
    int pawns_black = pawns_eg(pos2, NULL, NULL);
    v += pawns_white - pawns_black;

    // Step 5: Piece Mobility Evaluation
    int pieces_white = pieces_eg(pos, NULL, NULL);
    int pieces_black = pieces_eg(pos2, NULL, NULL);
    v += pieces_white - pieces_black;

    // Step 6: Mobility Evaluation
    int mobility_white = mobility_eg(pos, NULL, NULL);
    int mobility_black = mobility_eg(pos2, NULL, NULL);
    v += mobility_white - mobility_black;

    // Step 7: Threats Evaluation
    int threats_white = threats_eg(pos);
    int threats_black = threats_eg(pos2);
    v += threats_white - threats_black;

    // Step 8: Passed Pawns Evaluation
    int passed_white = passed_eg(pos, NULL, NULL);
    int passed_black = passed_eg(pos2, NULL, NULL);
    v += passed_white - passed_black;

    // Step 9: King Safety Evaluation
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

int scale_factor(ChessPosition *pos, int eg)
{
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    ChessPosition *pos_w = (eg > 0) ? pos : pos2;
    ChessPosition *pos_b = (eg > 0) ? pos2 : pos;

    int sf = 64; // Default scale factor
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

int phase(ChessPosition *pos)
{
    const int midgameLimit = 15258;
    const int endgameLimit = 3915;

    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);

    int npm = non_pawn_material(pos, NULL, NULL) + non_pawn_material(pos2, NULL, NULL);

    npm = fmax(endgameLimit, fmin(npm, midgameLimit));

    return ((npm - endgameLimit) * 128) /
           (midgameLimit - endgameLimit);
}

int tempo(ChessPosition *pos, Square *square)
{
    if (square != NULL)
    {
        return 0;
    }
    return 28 * (pos->w ? 1 : -1);
}
int rule(ChessPosition *pos, Square *square)
{
    if (square != NULL)
    {
        return 0;
    }
    return pos->m[0];
}