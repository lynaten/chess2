#ifndef SPACE_H
#define SPACE_H
#include "chess.h"
#include "material.h"
#include "attack.h"
#include "helper.h"
int space_area(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int space(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int pieceCount(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int blockedCount(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
#endif