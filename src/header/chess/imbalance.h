#ifndef IMBALANCE_H
#define IMBALANCE_H
#include "helper.h"
int imbalance(ChessPosition *pos, Square *square, void *param);
int bishop_pair(ChessPosition *pos, Square *square, void *param);
int imbalance_total(ChessPosition *pos);

#endif