#ifndef PIECE_H
#define PIECE_H
#include "chess.h"
#include "helper.h"
#include "king.h"
#include "mobility.h"
#include "attack.h"
int outpost(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int outpost_square(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int reachable_outpost(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int minor_behind_pawn(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int bishop_pawns(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int rook_on_file(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int trapped_rook(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int weak_queen(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int king_protector(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int long_diagonal_bishop(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int outpost_total(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int rook_on_queen_file(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int bishop_xray_pawns(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int rook_on_king_ring(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int queen_infiltration(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int pieces_mg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int pieces_eg(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int bishop_on_king_ring(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
#endif