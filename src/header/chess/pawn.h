#ifndef PAWN_H
#define PAWN_H
#include "chess.h"
#include "helper.h"
int isolated(ChessPosition *pos, Square *square, void *param);
int opposed(ChessPosition *pos, Square *square, void *param);
int phalanx(ChessPosition *pos, Square *square, void *param);
int supported(ChessPosition *pos, Square *square, void *param);
int backward(ChessPosition *pos, Square *square, void *param);
int doubled(ChessPosition *pos, Square *square, void *param);
int connected(ChessPosition *pos, Square *square, void *param);
int connected_bonus(ChessPosition *pos, Square *square, void *param);
int weak_unopposed_pawn(ChessPosition *pos, Square *square, void *param);
int weak_lever(ChessPosition *pos, Square *square, void *param);
int blocked(ChessPosition *pos, Square *square, void *param);
int doubled_isolated(ChessPosition *pos, Square *square, void *param);
int pawns_mg(ChessPosition *pos, Square *square, void *param);
int pawns_eg(ChessPosition *pos, Square *square, void *param);

#endif