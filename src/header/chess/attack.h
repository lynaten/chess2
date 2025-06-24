#ifndef ATTACK_H
#define ATTACK_H

#include "chess.h"
int pinned_direction(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int knight_attack(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int bishop_xray_attack(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int rook_xray_attack(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int queen_attack(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int pawn_attack(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int king_attack(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int attack(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
bool is_square_attacked(ChessPosition *pos, int x, int y, bool byWhite);
int queen_attack_diagonal(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int pinned(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);

#endif
