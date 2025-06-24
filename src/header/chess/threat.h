#ifndef THREAT_H
#define THREAT_H
#include "chess.h"
#include "attack.h"
#include "mobility.h"
#include "helper.h"
int safe_pawn(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int threat_safe_pawn(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int weak_enemies(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int minor_threat(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int rook_threat(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int hanging(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int king_threat(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int pawn_push_threat(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int slider_on_queen(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int knight_on_queen(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int restricted(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int weak_queen_protection(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int threats_mg(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int threats_eg(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);

#endif