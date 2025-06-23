#ifndef PASSEDPAWN_H
#define PASSEDPAWN_H
#include "chess.h"
#include "helper.h"
#include "attack.h"
#include "pawn.h"
int candidate_passed(ChessPosition *pos, Square *square, void *param);
int king_proximity(ChessPosition *pos, Square *square, void *param);
int passed_block(ChessPosition *pos, Square *square, void *param);
int passed_file(ChessPosition *pos, Square *square, void *param);
int passed_rank(ChessPosition *pos, Square *square, void *param);
int passed_leverable(ChessPosition *pos, Square *square, void *param);
int passed_mg(ChessPosition *pos, Square *square, void *param);
int passed_eg(ChessPosition *pos, Square *square, void *param);
#endif