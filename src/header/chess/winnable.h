#ifndef WINNABLE_H
#define WINNABLE_H
#include "chess.h"
#include <ctype.h>
#include "evaluation.h"
#include "material.h"
#include "passedPawn.h"
#include <limits.h>
int winnable(ChessPosition *pos);
int winnable_total_mg(ChessPosition *pos, void *param);
int winnable_total_eg(ChessPosition *pos, void *param);
#endif