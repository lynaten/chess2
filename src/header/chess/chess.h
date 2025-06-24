#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
/**
 * ChessPosition mirrors the JavaScript object:
 *
 *  b: 8×8 board (chars like 'p', 'P', 'r', '-', etc.)
 *  c: castling rights [Wkingside, Wqueenside, Bkingside, Bqueenside]
 *  e: en passant square, e.g. [file, rank], or [-1, -1] if none
 *  w: side to move (true=White, false=Black)
 *  m: move counters [halfmoveClock, fullmoveNumber]
 *
 * Additional fields:
 *  evaluationBar: integer representing current evaluation
 *  blackMoveCount, whiteMoveCount: custom counters if needed
 */

typedef struct
{
    int x; // File (column, 0-7)
    int y; // Rank (row, 0-7)
} Square;

typedef struct
{
    Square before;
    Square after;
    char piece_moved;
    char captured_piece;
    char promotion_piece;
    bool is_castling;
    bool is_en_passant;
} Move;

// mg eval:
//// piece value,
//// psqt,
//// imbalance_total:
////// imbalance,
////// bishop pair,
//// pawns,
//// pieces,
//// mobility,
//// threats:
////// hanging,
////// king_threat,
////// pawn_push_threat,
////// threat_safe_pawn,
////// slider on queen,
////// knight on queen,
////// restricted,
////// weak queen protection,
////// minor threat,
////// root threat,
//// passed,
//// space :
////// non pawn material,
////// space area,
//// king :
////// king danger:
//////// king_attackers_count,
//////// king_attackers_weight,
//////// king_attacks,
//////// weak_bonus,
//////// unsafe_checks,
//////// blockers_for_king,
//////// flank_attack,
//////// flank_defense,
//////// queen_count,
//////// knight_defender,
//////// shelter_strength,
//////// shelter_storm,
//////// mobility_mg,
//////// safe_check,
////// shelter_strength,
////// shelter_storm,
////// flank_attack,
////// pawnless_flank,
//// winnable total :
////// winnable:
//////// candidate_passed,
//////// non_pawn_material,
//////// satu lagi for loop,
// phase,
//// non_pawn_material,
// rule50,
// scale factor,
// tempo,

typedef struct
{
    int piece_value;           // none
    int psqt;                  // none
    int imbalance;             // none
    int bishop_pair;           // none
    int pawns;                 // none
    int pieces;                // none
    int mobility_white;        // none // double
    int mobility_black;        // none // d ok
    int hanging;               // none
    int king_threat;           // none
    int pawn_push_threat;      // none
    int threat_safe_pawn;      // none
    int slider_on_queen;       // none
    int knight_on_queen;       // none
    int restricted;            // none
    int weak_queen_protection; // none
    int minor_threat;          // none
    int root_threat;           // none
    int passed;                // none
    int non_pawn_material;     // none // double
    int piece_count;           // none
    int blocked_count;         // none
    int space_area;            // none
    int king_attackers_count;  // none
    int king_attackers_weight; // none
    int king_attacks;          // none
    int weak_bonus;            // none
    int unsafe_checks;         // none
    int blockers_for_king;     // none
    int flank_attack;          // none // double
    int flank_defense;         // none
    int queen_count;           // none
    int knight_defender;       // none
    int shelter_strength;      // none // double
    int shelter_storm;         // none // double
    int safe_check0;           // none
    int safe_check1;           // none
    int safe_check2;           // none
    int safe_check3;           // none
    int pawns_list[8];         // none
    int kx;                    // none
    int candidate_passed;      // none
    int phase;
    int rule50;
    int scale_factor;
    int tempo;
} EvalCacheMg;

typedef struct
{
    char b[8][8];      // Board
    bool c[4];         // Castling rights
    int e[2];          // En passant target
    bool w;            // Side to move
    int m[2];          // Move counters
    int evaluationBar; // Extra: evaluation “bar”
    EvalCacheMg eval;
    Move lastMove;
} ChessPosition;

/**
 * Returns the character at (x,y), or 'x' if out of bounds.
 */
char board(const ChessPosition *pos, int x, int y);

void colorflip(const ChessPosition *pos, ChessPosition *flipped);

int sum(const ChessPosition *pos,
        int (*func)(ChessPosition *, Square *, void *),
        void *param);

void displayBoard(ChessPosition pos);

int alphabetToIndex(char letter);

void move(Move inputMove, ChessPosition *pos);

#endif
