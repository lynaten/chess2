#ifndef MATERIAL_H
#define MATERIAL_H
#include "chess.h"
int non_pawn_material(ChessPosition *pos, Square *square, void *param);
int piece_value_bonus(ChessPosition *pos, Square *square, void *param);
int psqt_bonus(ChessPosition *pos, Square *square, void *param);
int piece_value_mg(ChessPosition *pos, Square *square, void *param);
int piece_value_eg(ChessPosition *pos, Square *square, void *param);
int psqt_mg(ChessPosition *pos, Square *square, void *param);
int psqt_eg(ChessPosition *pos, Square *square, void *param);
#endif