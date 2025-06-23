#ifndef HELPER_H
#define HELPER_H
#include "chess.h"
#include "pawn.h"
int rank(ChessPosition *pos, Square *square, void *param);
int file(ChessPosition *pos, Square *square, void *param);
int bishop_count(ChessPosition *pos, Square *square, void *param);
int queen_count(ChessPosition *pos, Square *square, void *param);
int pawn_count(ChessPosition *pos, Square *square, void *param);
int knight_count(ChessPosition *pos, Square *square, void *param);
int rook_count(ChessPosition *pos, Square *square, void *param);
int opposite_bishops(ChessPosition *pos);
int king_distance(ChessPosition *pos, Square *square, void *param);
int king_ring(ChessPosition *pos, Square *square, void *param);
int piece_count(ChessPosition *pos, Square *square, void *param);
int pawn_attacks_span(ChessPosition *pos, Square *square, void *param);
#endif
