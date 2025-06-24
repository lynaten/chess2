#ifndef KING_H
#define KING_H
#include "chess.h"
#include "attack.h"
#include "helper.h"
#include "mobility.h"
int pawnless_flank(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int king_pawn_distance(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int shelter_strength(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int shelter_storm(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int king_danger(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int king_mg(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int king_eg(ChessPosition *pos1, ChessPosition *pos2, bool colorflipped);
int strength_square(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int storm_square(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int check(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int safe_check(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int king_attackers_count(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int king_attackers_weight(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int king_attacks(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int weak_bonus(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int weak_squares(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int unsafe_checks(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int knight_defender(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int endgame_shelter(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int blockers_for_king(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int flank_attack(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
int flank_defense(ChessPosition *pos1, ChessPosition *pos2, Square *square, void *param, bool colorflipped);
#endif