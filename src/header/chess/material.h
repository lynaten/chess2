#ifndef MATERIAL_H
#define MATERIAL_H
#include "chess.h"
int non_pawn_material(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int piece_value_bonus(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int psqt_bonus(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int piece_value_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int piece_value_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int psqt_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int psqt_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
#endif