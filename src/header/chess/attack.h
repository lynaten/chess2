#ifndef ATTACK_H
#define ATTACK_H

#include "chess.h"
int pinned_direction(ChessPosition *pos, Square *square, void *param);
int knight_attack(ChessPosition *pos, Square *square, void *param);
int bishop_xray_attack(ChessPosition *pos, Square *square, void *param);
int rook_xray_attack(ChessPosition *pos, Square *square, void *param);
int queen_attack(ChessPosition *pos, Square *square, void *param);
int pawn_attack(ChessPosition *pos, Square *square, void *param);
int king_attack(ChessPosition *pos, Square *square, void *param);
int attack(ChessPosition *pos, Square *square, void *param);
bool is_square_attacked(ChessPosition *pos, int x, int y, bool byWhite);
int queen_attack_diagonal(ChessPosition *pos, Square *square, void *param);
int pinned(ChessPosition *pos, Square *square, void *param);

#endif
