#ifndef HELPER_H
#define HELPER_H
#include "chess.h"
#include "pawn.h"
int rank(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int file(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int bishop_count(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int queen_count(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int pawn_count(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int knight_count(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int rook_count(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int opposite_bishops(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int king_distance(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int king_ring(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int piece_count(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int pawn_attacks_span(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
#endif
