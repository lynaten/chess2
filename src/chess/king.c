#include "header/chess/king.h"

int pawn_file_count(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, pawn_file_count, NULL);
    }

    int x = square->x;
    int y = square->y;
    char piece = board(pos, x, y);

    if (toupper(piece) == 'P')
    {
        pos->eval.pawns_list[x] += 1;
    }

    return 0;
}

int king_file(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, king_file, NULL);
    }

    int x = square->x;
    int y = square->y;
    char piece = board(pos, x, y);

    if (piece == 'k')
    {
        pos->eval.kx = x;
    }

    return 0;
}

int pawnless_flank(ChessPosition *pos)
{
    pawn_file_count(pos,NULL,NULL);
    king_file(pos,NULL,NULL);
    int sum = 0;
    int kx = pos->eval.kx;
    int *pawns = pos->eval.pawns_list;

    if (kx == 0)
        sum = pawns[0] + pawns[1] + pawns[2];
    else if (kx < 3)
        sum = pawns[0] + pawns[1] + pawns[2] + pawns[3];
    else if (kx < 5)
        sum = pawns[2] + pawns[3] + pawns[4] + pawns[5];
    else if (kx < 7)
        sum = pawns[4] + pawns[5] + pawns[6] + pawns[7];
    else
        sum = pawns[5] + pawns[6] + pawns[7];

    return (sum == 0) ? 1 : 0;
}

int king_pawn_distance(ChessPosition *pos, Square *square, void *param)
{
    int v = 6, kx = 0, ky = 0, px = 0, py = 0;

    // Locate the king
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'K')
            {
                kx = x;
                ky = y;
            }
        }
    }

    // Locate the closest pawn
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            int dist = MAX(abs(x - kx), abs(y - ky));
            if (board(pos, x, y) == 'P' && dist < v)
            {
                px = x;
                py = y;
                v = dist;
            }
        }
    }

    if (square == NULL || (square->x == px && square->y == py))
    {
        return v;
    }
    return 0;
}

int shelter_strength(ChessPosition *pos, Square *square, void *param)
{
    int w = 0, s = 1024, tx = -1;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {

            char piece = board(pos, x, y);
            if (piece == 'k' || (pos->c[2] && x == 6 && y == 0) || (pos->c[3] && x == 2 && y == 0))
            {

                int w1 = strength_square(pos, &(Square){x, y}, NULL);
                int s1 = storm_square(pos, &(Square){x, y}, &(int){0});

                if (s1 - w1 < s - w)
                {
                    w = w1;
                    s = s1;
                    tx = MAX(1, MIN(6, x));
                }
            }
        }
    }

    if (square == NULL)
        return w;

    if (tx >= 0 && board(pos, square->x, square->y) == 'p' && square->x >= tx - 1 && square->x <= tx + 1)
    {
        for (int y = square->y - 1; y >= 0; y--)
        {
            if (board(pos, square->x, y) == 'p')
                return 0;
        }
        return 1;
    }
    return 0;
}

int shelter_storm(ChessPosition *pos, Square *square, void *param)
{
    int w = 0, s = 1024, tx = -1;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            char piece = board(pos, x, y);
            if (piece == 'k' || (pos->c[2] && x == 6 && y == 0) || (pos->c[3] && x == 2 && y == 0))
            {
                int w1 = strength_square(pos, &(Square){x, y}, NULL);
                int s1 = storm_square(pos, &(Square){x, y}, &(int){0});
                if (s1 - w1 < s - w)
                {
                    w = w1;
                    s = s1;
                    tx = MAX(1, MIN(6, x));
                }
            }
        }
    }

    if (square == NULL)
        return s;
    if (tx >= 0 && toupper(board(pos, square->x, square->y)) == 'P' && square->x >= tx - 1 && square->x <= tx + 1)
    {
        for (int y = square->y - 1; y >= 0; y--)
        {
            if (board(pos, square->x, y) == board(pos, square->x, square->y))
                return 0;
        }
        return 1;
    }
    return 0;
}

#include <stdio.h>

int king_danger(ChessPosition *pos)
{

    // Individual components
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    int count = king_attackers_count(pos, NULL, NULL);
    pos->eval.king_attackers_count = count;

    int weight = king_attackers_weight(pos, NULL, NULL);
    pos->eval.king_attackers_weight = weight;

    int kingAttacks = king_attacks(pos, NULL, NULL);
    pos->eval.king_attacks = kingAttacks;

    int weak = weak_bonus(pos, NULL, NULL);
    pos->eval.weak_bonus = weak;

    int unsafeChecks = unsafe_checks(pos, NULL, NULL);
    pos->eval.unsafe_checks = unsafeChecks;

    int blockersForKing = blockers_for_king(pos, NULL, NULL);
    pos->eval.blockers_for_king = blockersForKing;

    int kingFlankAttack = flank_attack(pos, NULL, NULL);
    pos->eval.flank_attack = kingFlankAttack;

    int kingFlankDefense = flank_defense(pos, NULL, NULL);
    pos->eval.flank_defense = kingFlankDefense;

    int noQueen = (queen_count(pos, NULL, NULL) > 0 ? 0 : 1);
    pos->eval.queen_count = queen_count(pos, NULL, NULL);

    int knightDefender = knight_defender(pos2, NULL, NULL);
    pos->eval.knight_defender = knightDefender;

    int shelterStrength = shelter_strength(pos, NULL, NULL);
    pos->eval.shelter_strength = shelterStrength;

    int shelterStorm = shelter_storm(pos, NULL, NULL);
    pos->eval.shelter_storm = shelterStorm;

    int mobilityWhite = pos->eval.mobility_white;

    int mobilityBlack = pos->eval.mobility_black;

    int safeCheck3 = safe_check(pos, NULL, &(int){3});
    pos->eval.safe_check3 = safeCheck3;

    int safeCheck2 = safe_check(pos, NULL, &(int){2});
    pos->eval.safe_check2 = safeCheck2;

    int safeCheck1 = safe_check(pos, NULL, &(int){1});
    pos->eval.safe_check1 = safeCheck1;

    int safeCheck0 = safe_check(pos, NULL, &(int){0});
    pos->eval.safe_check0 = safeCheck0;

    // Final calculation
    int v = count * weight +
            69 * kingAttacks +
            185 * weak -
            100 * (knightDefender > 0) +
            148 * unsafeChecks +
            98 * blockersForKing -
            4 * kingFlankDefense +
            ((3 * kingFlankAttack * kingFlankAttack / 8) << 0) -
            873 * noQueen -
            ((6 * (shelterStrength - shelterStorm) / 8) << 0) +
            mobilityWhite -
            mobilityBlack +
            37 +
            (772 * MIN(safeCheck3, 1.45)) +
            (1084 * MIN(safeCheck2, 1.75)) +
            (645 * MIN(safeCheck1, 1.50)) +
            (792 * MIN(safeCheck0, 1.62));

    // Final result
    if (v > 100)
    {
        return v;
    }
    else
    {
        return 0;
    }
}

int king_mg(ChessPosition *pos)
{
    int v = 0;

    int kd = king_danger(pos);

    int shelterStrength = pos->eval.shelter_strength;
    v -= shelterStrength;

    int shelterStorm = pos->eval.shelter_storm;
    v += shelterStorm;

    int dangerScore = (kd * kd) / 4096;
    v += dangerScore;

    int flankAttack = pos->eval.flank_attack;

    v += 8 * flankAttack;

    int pawnlessFlank = pawnless_flank(pos);
    v += 17 * pawnlessFlank;

    return v;
}

int king_eg(ChessPosition *pos)
{
    int v = 0;

    v -= 16 * king_pawn_distance(pos, NULL, NULL);
    v += endgame_shelter(pos, NULL, NULL);
    v += 95 * pawnless_flank(pos);
    v += king_danger(pos) / 16;

    return v;
}

int strength_square(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, strength_square, param);

    int v = 5;
    int kx = fmin(6, fmax(1, square->x));
    int weakness[4][7] = {
        {-6, 81, 93, 58, 39, 18, 25},
        {-43, 61, 35, -49, -29, -11, -63},
        {-10, 75, 23, -2, 32, 3, -45},
        {-39, -13, -29, -52, -48, -67, -166}};

    for (int x = kx - 1; x <= kx + 1; x++)
    {
        int us = 0;
        for (int y = 7; y >= square->y; y--)
        {
            if (board(pos, x, y) == 'p' &&
                board(pos, x - 1, y + 1) != 'P' &&
                board(pos, x + 1, y + 1) != 'P')
            {
                us = y;
            }
        }
        int f = fmin(x, 7 - x);
        v += (us >= 0 && us < 7) ? weakness[f][us] : 0;
    }

    return v;
}

int storm_square(ChessPosition *pos, Square *square, void *param)
{

    if (square == NULL)
        return sum(pos, storm_square, param);

    int eg = *(int *)param;

    int v = 0, ev = 5;
    int kx = fmin(6, fmax(1, square->x));
    int unblockedstorm[4][7] = {
        {85, -289, -166, 97, 50, 45, 50},
        {46, -25, 122, 45, 37, -10, 20},
        {-6, 51, 168, 34, -2, -22, -14},
        {-15, -11, 101, 4, 11, -15, -29}};
    int blockedstorm[2][7] = {
        {0, 0, 76, -10, -7, -4, -1},
        {0, 0, 78, 15, 10, 6, 2}};

    for (int x = kx - 1; x <= kx + 1; x++)
    {
        int us = 0, them = 0;
        for (int y = 7; y >= square->y; y--)
        {
            if (board(pos, x, y) == 'p' &&
                board(pos, x - 1, y + 1) != 'P' &&
                board(pos, x + 1, y + 1) != 'P')
            {
                us = y;
            }
            if (board(pos, x, y) == 'P')
            {
                them = y;
            }
        }
        int f = fmin(x, 7 - x);
        if (us > 0 && them == us + 1)
        {
            v += blockedstorm[0][them];
            ev += blockedstorm[1][them];
        }
        else
        {
            v += unblockedstorm[f][them];
        }
    }

    return eg ? ev : v;
}

int check(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, check, param);
    int type = *(int *)param;

    if ((rook_xray_attack(pos, square, NULL) && (param == NULL || type == 2 || type == 4)) ||
        (queen_attack(pos, square, NULL) && (param == NULL || type == 3)))
    {
        for (int i = 0; i < 4; i++)
        {
            int ix = (i == 0 ? -1 : i == 1 ? 1
                                           : 0);
            int iy = (i == 2 ? -1 : i == 3 ? 1
                                           : 0);
            for (int d = 1; d < 8; d++)
            {
                char b = board(pos, square->x + d * ix, square->y + d * iy);
                if (b == 'k')
                    return 1;
                if (b != '-' && b != 'q')
                    break;
            }
        }
    }

    if ((bishop_xray_attack(pos, square, NULL) && (param == NULL || type == 1 || type == 4)) ||
        (queen_attack(pos, square, NULL) && (param == NULL || type == 3)))
    {

        for (int i = 0; i < 4; i++)
        {

            int ix = ((i > 1) * 2 - 1);
            int iy = ((i % 2 == 0) * 2 - 1);
            for (int d = 1; d < 8; d++)
            {
                char b = board(pos, square->x + d * ix, square->y + d * iy);
                if (b == 'k')
                    return 1;
                if (b != '-' && b != 'q')
                    break;
            }
        }
    }
    if (knight_attack(pos, square, NULL) && (param == NULL || type == 0 || type == 4))
    {
        if (board(pos, square->x + 2, square->y + 1) == 'k' ||
            board(pos, square->x + 2, square->y - 1) == 'k' ||
            board(pos, square->x + 1, square->y + 2) == 'k' ||
            board(pos, square->x + 1, square->y - 2) == 'k' ||
            board(pos, square->x - 2, square->y + 1) == 'k' ||
            board(pos, square->x - 2, square->y - 1) == 'k' ||
            board(pos, square->x - 1, square->y + 2) == 'k' ||
            board(pos, square->x - 1, square->y - 2) == 'k')
        {
            return 1;
        }
    }

    return 0;
}

int safe_check(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, safe_check, param);
    int type = *(int *)param;

    if (strchr("PNBRQK", board(pos, square->x, square->y)))
        return 0;

    if (!check(pos, square, &(int){type}))
        return 0;

    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);

    if ((type == 3 && safe_check(pos, square, &(int){2})) ||
        (type == 1 && safe_check(pos, square, &(int){3})))
    {
        return 0;
    }

    if ((!attack(pos2, &(Square){square->x, 7 - square->y}, NULL) ||
         (weak_squares(pos, square, NULL) && attack(pos, square, NULL) > 1)) &&
        (type != 3 || !queen_attack(pos2, &(Square){square->x, 7 - square->y}, NULL)))
    {
        return 1;
    }

    return 0;
}

int king_attackers_count(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, king_attackers_count, param);

    char piece = board(pos, square->x, square->y);

    if (strchr("PNBRQ", piece) == NULL)
        return 0;

    if (piece == 'P')
    {
        int v = 0;
        for (int dir = -1; dir <= 1; dir += 2)
        {
            int fr = (board(pos, square->x + dir * 2, square->y) == 'P');
            if (square->x + dir >= 0 && square->x + dir <= 7 &&
                king_ring(pos, &(Square){square->x + dir, square->y - 1}, &(int){1}))
            {
                v += (fr ? 0.5 : 1);
            }
        }
        return v;
    }

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            Square s2 = {x, y};
            if (s2.x < 0 || s2.x >= 8 || s2.y < 0 || s2.y >= 8)
            {
                continue;
            }

            if (king_ring(pos, &s2, &(int){0}))
            {
                if (knight_attack(pos, &s2, square))
                {
                    return 1;
                }
                if (bishop_xray_attack(pos, &s2, square))
                {
                    return 1;
                }
                if (rook_xray_attack(pos, &s2, square))
                {
                    return 1;
                }
                if (queen_attack(pos, &s2, square))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int king_attackers_weight(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, king_attackers_weight, param);

    if (king_attackers_count(pos, square, NULL))
    {
        char piece = board(pos, square->x, square->y);
        int weights[] = {0, 81, 52, 44, 10}; // Weights for P, N, B, R, Q
        char *pieces = "PNBRQ";
        char *found = strchr(pieces, piece);
        if (found)
            return weights[found - pieces];
    }

    return 0;
}

int king_attacks(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, king_attacks, param);

    char piece = board(pos, square->x, square->y);
    if (strchr("NBRQ", piece) == NULL)
        return 0;

    if (king_attackers_count(pos, square, NULL) == 0)
        return 0;

    int kx = 0, ky = 0, v = 0;

    // Find king's position
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'k')
            {
                kx = x;
                ky = y;
                break;
            }
        }
    }

    // Check surrounding squares of the king
    for (int x = kx - 1; x <= kx + 1; x++)
    {
        for (int y = ky - 1; y <= ky + 1; y++)
        {
            if (x >= 0 && y >= 0 && x <= 7 && y <= 7 && (x != kx || y != ky))
            {
                Square s2 = {x, y};
                v += knight_attack(pos, &s2, square);
                v += bishop_xray_attack(pos, &s2, square);
                v += rook_xray_attack(pos, &s2, square);
                v += queen_attack(pos, &s2, square);
            }
        }
    }

    return v;
}

int weak_bonus(ChessPosition *pos, Square *square, void *param)
{

    if (square == NULL)
        return sum(pos, weak_bonus, param);
    if (!weak_squares(pos, square, NULL))
        return 0;
    if (!king_ring(pos, square, &(int){0}))
        return 0;
    return 1;
}

int weak_squares(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, weak_squares, param);

    if (attack(pos, square, NULL))
    {
        ChessPosition flipped;
        ChessPosition *pos2 = &flipped;
        colorflip(pos, pos2);
        int attack_count = attack(pos2, &(Square){square->x, 7 - square->y}, NULL);

        if (attack_count >= 2)
        {
            return 0;
        }

        if (attack_count == 0)
        {
            return 1;
        }

        if (king_attack(pos2, &(Square){square->x, 7 - square->y}, NULL) ||
            queen_attack(pos2, &(Square){square->x, 7 - square->y}, NULL))
        {
            return 1;
        }
    }

    return 0;
}

int unsafe_checks(ChessPosition *pos, Square *square, void *param)
{

    if (square == NULL)
        return sum(pos, unsafe_checks, param);

    if (check(pos, square, &(int){0}) && safe_check(pos, NULL, &(int){0}) == 0)
        return 1;

    if (check(pos, square, &(int){1}) && safe_check(pos, NULL, &(int){1}) == 0)
        return 1;

    if (check(pos, square, &(int){2}) && safe_check(pos, NULL, &(int){2}) == 0)
        return 1;

    return 0;
}

int knight_defender(ChessPosition *pos, Square *square, void *param)
{

    if (square == NULL)
        return sum(pos, knight_defender, param);

    if (knight_attack(pos, square, NULL) && king_attack(pos, square, NULL))
    {
        return 1;
    }

    return 0;
}

int endgame_shelter(ChessPosition *pos, Square *square, void *param)
{
    int w = 0, s = 1024, e = 0;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'k' ||
                (pos->c[2] && x == 6 && y == 0) ||
                (pos->c[3] && x == 2 && y == 0))
            {
                int w1 = strength_square(pos, &(Square){x, y}, NULL);
                int s1 = storm_square(pos, &(Square){x, y}, &(int){0});
                int e1 = storm_square(pos, &(Square){x, y}, &(int){1});

                if (s1 - w1 < s - w)
                {
                    w = w1;
                    s = s1;
                    e = e1;
                }
            }
        }
    }

    if (square == NULL)
        return e;

    return 0;
}

int blockers_for_king(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, blockers_for_king, param);

    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);

    if (pinned_direction(pos2, &(Square){square->x, 7 - square->y}, NULL))
    {
        return 1;
    }
    return 0;
}

int flank_attack(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, flank_attack, param);

    if (square->y > 4)
        return 0;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'k')
            {
                if ((x == 0 && square->x > 2) ||
                    (x < 3 && square->x > 3) ||
                    (x >= 3 && x < 5 && (square->x < 2 || square->x > 5)) ||
                    (x >= 5 && square->x < 4) ||
                    (x == 7 && square->x < 5))
                {
                    return 0;
                }
            }
        }
    }

    int a = attack(pos, square, NULL);
    if (!a)
        return 0;

    return a > 1 ? 2 : 1;
}

int flank_defense(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
        return sum(pos, flank_defense, param);

    if (square->y > 4)
        return 0;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'k')
            {
                if ((x == 0 && square->x > 2) ||
                    (x < 3 && square->x > 3) ||
                    (x >= 3 && x < 5 && (square->x < 2 || square->x > 5)) ||
                    (x >= 5 && square->x < 4) ||
                    (x == 7 && square->x < 5))
                {
                    return 0;
                }
            }
        }
    }

    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    int result = attack(pos2, &(Square){square->x, 7 - square->y}, NULL) > 0 ? 1 : 0;

    return result;
}
