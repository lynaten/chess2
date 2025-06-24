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

int main_evaluation(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int middle_game_evaluation(ChessPosition *pos1, ChessPosition *pos2, bool nowinnablepos2, bool colorflipped);
int end_game_evaluation(ChessPosition *pos1, ChessPosition *pos2, bool nowinnablepos2, bool colorflipped);
int scale_factor(ChessPosition *pos1, ChessPosition *pos2, int eg, bool colorflipped);
int phase(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int tempo(ChessPosition *pos1, ChessPosition *pos2, Square *square, bool colorflipped);
int rule(ChessPosition *pos1, ChessPosition *pos2, Square *square, bool colorflipped);

#endif