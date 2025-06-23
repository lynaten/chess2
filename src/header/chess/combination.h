#ifndef COMBINATION_H
#define COMBINATION_H

#include "chess.h"
#include "attack.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Move *queen_moves(ChessPosition *pos, Square *square, void *param, int *move_count);
Move *knight_moves(ChessPosition *pos, Square *square, void *param, int *moveCount);
Move *bishop_moves(ChessPosition *pos, Square *square, void *param, int *moveCount);
Move *rook_moves(ChessPosition *pos, Square *square, void *param, int *moveCount);
Move *pawn_moves(ChessPosition *pos, Square *square, void *param, int *moveCount);
Move *king_moves(ChessPosition *pos, Square *square, void *param, int *moveCount);
Move *combination(ChessPosition *pos, int *totalMoves);

#endif
