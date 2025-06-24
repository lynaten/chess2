#ifndef PASSEDPAWN_H
#define PASSEDPAWN_H
#include "chess.h"
#include "helper.h"
#include "attack.h"
#include "pawn.h"
int candidate_passed(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int king_proximity(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int passed_block(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int passed_file(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int passed_rank(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int passed_leverable(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int passed_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int passed_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
#endif