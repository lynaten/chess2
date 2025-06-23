#ifndef PIECE_H
#define PIECE_H
#include "chess.h"
#include "helper.h"
#include "king.h"
#include "mobility.h"
#include "attack.h"
int outpost(ChessPosition *pos, Square *square, void *param);
int outpost_square(ChessPosition *pos, Square *square, void *param);
int reachable_outpost(ChessPosition *pos, Square *square, void *param);
int minor_behind_pawn(ChessPosition *pos, Square *square, void *param);
int bishop_pawns(ChessPosition *pos, Square *square, void *param);
int rook_on_file(ChessPosition *pos, Square *square, void *param);
int trapped_rook(ChessPosition *pos, Square *square, void *param);
int weak_queen(ChessPosition *pos, Square *square, void *param);
int king_protector(ChessPosition *pos, Square *square, void *param);
int long_diagonal_bishop(ChessPosition *pos, Square *square, void *param);
int outpost_total(ChessPosition *pos, Square *square, void *param);
int rook_on_queen_file(ChessPosition *pos, Square *square, void *param);
int bishop_xray_pawns(ChessPosition *pos, Square *square, void *param);
int rook_on_king_ring(ChessPosition *pos, Square *square, void *param);
int queen_infiltration(ChessPosition *pos, Square *square, void *param);
int pieces_mg(ChessPosition *pos, Square *square, void *param);
int pieces_eg(ChessPosition *pos, Square *square, void *param);
int bishop_on_king_ring(ChessPosition *pos, Square *square, void *param);
#endif