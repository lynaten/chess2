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

int middle_game_evaluation(ChessPosition *pos, bool nowinnable)
{
    int v = 0;
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2); // pos2 sekarang adalah board yang sama tapi warnanya dibalik

    // Piece value
    int white_piece_value = piece_value_mg(pos, NULL, NULL);
    int black_piece_value = piece_value_mg(pos2, NULL, NULL);
    pos->eval.piece_value = white_piece_value - black_piece_value;
    pos2->eval.piece_value = black_piece_value - white_piece_value; // Nilai terbalik untuk pos2
    v += white_piece_value - black_piece_value;

    // Piece-square table
    int white_psqt = psqt_mg(pos, NULL, NULL);
    int black_psqt = psqt_mg(pos2, NULL, NULL);
    pos->eval.psqt = white_psqt - black_psqt;
    pos2->eval.psqt = black_psqt - white_psqt; // Nilai terbalik untuk pos2
    v += white_psqt - black_psqt;

    // Imbalance - Ini adalah nilai absolut, jadi sama untuk kedua sisi, tapi untuk v, itu relatif ke sisi saat ini
    int imbalance_val = imbalance_total(pos); // Hitung sekali
    pos->eval.imbalance = imbalance_val;
    pos2->eval.imbalance = imbalance_val; // Imbalance biasanya simetris
    v += imbalance_val; // Tambahkan ke total evaluasi

    // Pawn structure
    int white_pawns = pawns_mg(pos, NULL, NULL);
    int black_pawns = pawns_mg(pos2, NULL, NULL);
    pos->eval.pawns = white_pawns - black_pawns;
    pos2->eval.pawns = black_pawns - white_pawns; // Nilai terbalik untuk pos2
    v += white_pawns - black_pawns;

    // Pieces
    int white_pieces = pieces_mg(pos, NULL, NULL);
    int black_pieces = pieces_mg(pos2, NULL, NULL);
    pos->eval.pieces = white_pieces - black_pieces;
    pos2->eval.pieces = black_pieces - white_pieces; // Nilai terbalik untuk pos2
    v += white_pieces - black_pieces;

    // Mobility
    int white_mobility = mobility_mg(pos, NULL, NULL);
    int black_mobility = mobility_mg(pos2, NULL, NULL);
    pos->eval.mobility_white = white_mobility; // Simpan nilai mentah putih
    pos->eval.mobility_black = black_mobility; // Simpan nilai mentah hitam
    pos2->eval.mobility_white = black_mobility; // Untuk pos2, mobility putih adalah mobility_black asli
    pos2->eval.mobility_black = white_mobility; // Untuk pos2, mobility hitam adalah mobility_white asli
    v += white_mobility - black_mobility;

    // Threats
    int white_threats = threats_mg(pos);
    int black_threats = threats_mg(pos2);
    v += white_threats - black_threats;

    // Passed pawns
    int white_passed = passed_mg(pos, NULL, NULL);
    int black_passed = passed_mg(pos2, NULL, NULL);
    pos->eval.passed = white_passed - black_passed;
    pos2->eval.passed = black_passed - white_passed; // Nilai terbalik untuk pos2
    v += white_passed - black_passed;

    // Space
    int white_space = space(pos);
    int black_space = space(pos2);
    v += white_space - black_space;

    // King safety
    int white_king = king_mg(pos);
    int black_king = king_mg(pos2);
    v += white_king - black_king;

    // Winnable positions
    if (!nowinnable)
    {
        // winnable_total_mg mungkin juga harus menyimpan breakdown ke pos->eval
        // Jika winnable_total_mg juga menggunakan pos dan pos2 secara internal,
        // Anda mungkin perlu lebih teliti. Namun, untuk contoh ini, saya asumsikan
        // itu hanya mengembalikan nilai tunggal berdasarkan 'pos'
        int winnable_val = winnable_total_mg(pos, &(int){v});
        v += winnable_val;
    }

    // Pastikan pos->evaluationBar juga diatur
    pos->evaluationBar = v;
    pos2->evaluationBar = -v; // Untuk pos2, nilai total evaluasi adalah kebalikannya

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