#ifndef KING_H
#define KING_H
#include "chess.h"
#include "attack.h"
#include "helper.h"
#include "mobility.h"
int pawnless_flank(ChessPosition *pos);
int king_pawn_distance(ChessPosition *pos, Square *square, void *param);
int shelter_strength(ChessPosition *pos, Square *square, void *param);
int shelter_storm(ChessPosition *pos, Square *square, void *param);
int king_danger(ChessPosition *pos);
int king_mg(ChessPosition *pos);
int king_eg(ChessPosition *pos);
int strength_square(ChessPosition *pos, Square *square, void *param);
int storm_square(ChessPosition *pos, Square *square, void *param);
int check(ChessPosition *pos, Square *square, void *param);
int safe_check(ChessPosition *pos, Square *square, void *param);
int king_attackers_count(ChessPosition *pos, Square *square, void *param);
int king_attackers_weight(ChessPosition *pos, Square *square, void *param);
int king_attacks(ChessPosition *pos, Square *square, void *param);
int weak_bonus(ChessPosition *pos, Square *square, void *param);
int weak_squares(ChessPosition *pos, Square *square, void *param);
int unsafe_checks(ChessPosition *pos, Square *square, void *param);
int knight_defender(ChessPosition *pos, Square *square, void *param);
int endgame_shelter(ChessPosition *pos, Square *square, void *param);
int blockers_for_king(ChessPosition *pos, Square *square, void *param);
int flank_attack(ChessPosition *pos, Square *square, void *param);
int flank_defense(ChessPosition *pos, Square *square, void *param);
#endif