#ifndef THREAT_H
#define THREAT_H
#include "chess.h"
#include "attack.h"
#include "mobility.h"
#include "helper.h"
int safe_pawn(ChessPosition *pos, Square *square, void *param);
int threat_safe_pawn(ChessPosition *pos, Square *square, void *param);
int weak_enemies(ChessPosition *pos, Square *square, void *param);
int minor_threat(ChessPosition *pos, Square *square, void *param);
int rook_threat(ChessPosition *pos, Square *square, void *param);
int hanging(ChessPosition *pos, Square *square, void *param);
int king_threat(ChessPosition *pos, Square *square, void *param);
int pawn_push_threat(ChessPosition *pos, Square *square, void *param);
int slider_on_queen(ChessPosition *pos, Square *square, void *param);
int knight_on_queen(ChessPosition *pos, Square *square, void *param);
int restricted(ChessPosition *pos, Square *square, void *param);
int weak_queen_protection(ChessPosition *pos, Square *square, void *param);
int minor_threat_eval(ChessPosition *pos, Square *square, void *param);
int rook_threat_eval(ChessPosition *pos, Square *square, void *param);
int threats_mg(ChessPosition *pos);
int threats_eg(ChessPosition *pos);

#endif