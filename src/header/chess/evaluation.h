#ifndef EVALUATION_H
#define EVALUATION_H
#include "chess.h"
#include <limits.h>
#include <stdint.h>
#include "helper.h"
#include "passedPawn.h"
#include "material.h"
#include "winnable.h"
#include "king.h"
#include "threat.h"
#include "space.h"
#include "piece.h"
#include "imbalance.h"

int main_evaluation(ChessPosition *pos);
int middle_game_evaluation(ChessPosition *pos, bool nowinnable);
int end_game_evaluation(ChessPosition *pos, bool nowinnable);
int scale_factor(ChessPosition *pos, int eg);
int phase(ChessPosition *pos);
int tempo(ChessPosition *pos, Square *square);
int rule(ChessPosition *pos, Square *square);

#endif