#ifndef IMBALANCE_H
#define IMBALANCE_H
#include "helper.h"
int imbalance(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int bishop_pair(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int imbalance_total(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);

#endif