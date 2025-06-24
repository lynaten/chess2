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
    int piece_value;
    int psqt;
    int imbalance;
    int bishop_pair;
    int pawns;
    int pieces;
    int mobility_white;
    int mobility_black;
    int hanging;
    int king_threat;
    int pawn_push_threat;
    int threat_safe_pawn;
    int slider_on_queen;
    int knight_on_queen;
    int restricted;
    int weak_queen_protection;
    int minor_threat;
    int root_threat;
    int passed;
    int non_pawn_material;
    int piece_count;
    int blocked_count;
    int space_area;
    int king_attackers_count;
    int king_attackers_weight;
    int king_attacks;
    int weak_bonus;
    int unsafe_checks;
    int blockers_for_king;
    int flank_attack;
    int flank_defense;
    int queen_count;
    int knight_defender;
    int shelter_strength;
    int shelter_storm;
    int safe_check0;
    int safe_check1;
    int safe_check2;
    int safe_check3;
    int pawns_list[8];
    int kx;
    int candidate_passed;
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
    // EvalCacheMg eval;
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
