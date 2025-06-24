#ifndef MOBILITY_H
#define MOBILITY_H
#include "chess.h"
#include "attack.h"
#include "king.h"
#include <stddef.h>
#include <ctype.h>
int mobility_area(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int mobility(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int mobility_bonus(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int mobility_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int mobility_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
#endif