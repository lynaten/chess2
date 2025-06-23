#ifndef SPACE_H
#define SPACE_H
#include "chess.h"
#include "material.h"
#include "attack.h"
#include "helper.h"
int space_area(ChessPosition *pos, Square *square, void *param);
int space(ChessPosition *pos);
int pieceCount(ChessPosition *pos, Square *square, void *param);
int blockedCount(ChessPosition *pos, Square *square, void *param);
#endif