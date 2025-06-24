#ifndef PAWN_H
#define PAWN_H
#include "chess.h"
#include "helper.h"
int isolated(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int opposed(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int phalanx(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int supported(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int backward(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int doubled(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int connected(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int connected_bonus(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int weak_unopposed_pawn(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int weak_lever(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int blocked(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int doubled_isolated(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int pawns_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int pawns_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);

#endif