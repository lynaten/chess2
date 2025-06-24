#ifndef WINNABLE_H
#define WINNABLE_H
#include "chess.h"
#include <ctype.h>
#include "evaluation.h"
#include "material.h"
#include "passedPawn.h"
#include <limits.h>
int winnable(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int winnable_total_mg(ChessPosition *pos1, ChessPosition *pos2, void *param, bool colorflipped);
int winnable_total_eg(ChessPosition *pos1, ChessPosition *pos2, void *param, bool colorflipped);
#endif