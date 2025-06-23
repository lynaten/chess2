#ifndef MOBILITY_H
#define MOBILITY_H
#include "chess.h"
#include "attack.h"
#include "king.h"
#include <stddef.h>
#include <ctype.h>
int mobility_area(ChessPosition *pos, Square *square, void *param);
int mobility(ChessPosition *pos, Square *square, void *param);
int mobility_bonus(ChessPosition *pos, Square *square, void *param);
int mobility_mg(ChessPosition *pos, Square *square, void *param);
int mobility_eg(ChessPosition *pos, Square *square, void *param);
#endif