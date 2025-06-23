#include "header/chess/threat.h"

int safe_pawn(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, safe_pawn, NULL);
    }
    if (board(pos, square->x, square->y) != 'P')
    {
        return 0;
    }
    if (attack(pos, square, NULL))
    {
        return 1;
    }
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    if (!attack(pos2, &(Square){square->x, 7 - square->y}, NULL))
    {
        return 1;
    }
    return 0;
}

int threat_safe_pawn(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, threat_safe_pawn, NULL);
    }
    if (strchr("nbrq", board(pos, square->x, square->y)) == NULL)
    {
        return 0;
    }
    if (!pawn_attack(pos, square, NULL))
    {
        return 0;
    }
    if (safe_pawn(pos, &(Square){square->x - 1, square->y + 1}, NULL) || safe_pawn(pos, &(Square){square->x + 1, square->y + 1}, NULL))
    {
        return 1;
    }
    return 0;
}

int weak_enemies(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, weak_enemies, NULL);
    }
    if (strchr("pnbrqk", board(pos, square->x, square->y)) == NULL)
    {
        return 0;
    }
    if (board(pos, square->x - 1, square->y - 1) == 'p' || board(pos, square->x + 1, square->y - 1) == 'p')
    {
        return 0;
    }
    if (!attack(pos, square, NULL))
    {
        return 0;
    }
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    if (attack(pos, square, NULL) <= 1 && attack(pos2, &(Square){square->x, 7 - square->y}, NULL) > 1)
    {
        return 0;
    }
    return 1;
}

int minor_threat(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, minor_threat, NULL);
    }

    int type = strchr("pnbrqk", board(pos, square->x, square->y)) - "pnbrqk";
    if (type < 0)
    {
        return 0;
    }

    if (!knight_attack(pos, square, NULL) && !bishop_xray_attack(pos, square, NULL))
    {
        return 0;
    }
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    if ((board(pos, square->x, square->y) == 'p' || !(board(pos, square->x - 1, square->y - 1) == 'p' || board(pos, square->x + 1, square->y - 1) == 'p' || (attack(pos, square, NULL) <= 1 && attack(pos2, &(Square){square->x, 7 - square->y}, NULL) > 1))) && !weak_enemies(pos, square, NULL))
    {
        return 0;
    }

    return type + 1;
}

int rook_threat(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, rook_threat, NULL);
    }
    int type = strchr("pnbrqk", board(pos, square->x, square->y)) - "pnbrqk";
    if (type < 0)
    {
        return 0;
    }
    if (!weak_enemies(pos, square, NULL))
    {
        return 0;
    }
    if (!rook_xray_attack(pos, square, NULL))
    {
        return 0;
    }
    return type + 1;
}

int hanging(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, hanging, NULL);
    }
    if (!weak_enemies(pos, square, NULL))
    {
        return 0;
    }
    if (board(pos, square->x, square->y) != 'p' && attack(pos, square, NULL) > 1)
    {
        return 1;
    }
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    if (!attack(pos2, &(Square){square->x, 7 - square->y}, NULL))
    {
        return 1;
    }
    return 0;
}

int king_threat(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, king_threat, NULL);
    }
    if (strchr("pnbrq", board(pos, square->x, square->y)) == NULL)
    {
        return 0;
    }
    if (!weak_enemies(pos, square, NULL))
    {
        return 0;
    }
    if (!king_attack(pos, square, NULL))
    {
        return 0;
    }
    return 1;
}

int pawn_push_threat(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, pawn_push_threat, NULL);
    }
    if (strchr("pnbrqk", board(pos, square->x, square->y)) == NULL)
    {
        return 0;
    }
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    for (int ix = -1; ix <= 1; ix += 2)
    {
        if (board(pos, square->x + ix, square->y + 2) == 'P' && board(pos, square->x + ix, square->y + 1) == '-' && board(pos, square->x + ix - 1, square->y) != 'p' && board(pos, square->x + ix + 1, square->y) != 'p' && (attack(pos, &(Square){square->x + ix, square->y + 1}, NULL) || !attack(pos2, &(Square){square->x + ix, 6 - square->y}, NULL)))
        {
            return 1;
        }
        if (square->y == 3 && board(pos, square->x + ix, square->y + 3) == 'P' && board(pos, square->x + ix, square->y + 2) == '-' && board(pos, square->x + ix, square->y + 1) == '-' && board(pos, square->x + ix - 1, square->y) != 'p' && board(pos, square->x + ix + 1, square->y) != 'p' && (attack(pos, &(Square){square->x + ix, square->y + 1}, NULL) || !attack(pos2, &(Square){square->x + ix, 6 - square->y}, NULL)))
        {
            return 1;
        }
    }
    return 0;
}

int slider_on_queen(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, slider_on_queen, NULL);
    }

    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    if (queen_count(pos2, NULL, NULL) != 1)
        return 0;
    if (board(pos, square->x, square->y) == 'P')
        return 0;
    if (board(pos, square->x - 1, square->y - 1) == 'p')
        return 0;
    if (board(pos, square->x + 1, square->y - 1) == 'p')
        return 0;
    if (attack(pos, square, NULL) <= 1)
        return 0;
    if (!mobility_area(pos, square, NULL))
        return 0;

    int diagonal = queen_attack_diagonal(pos2, &(Square){square->x, 7 - square->y}, NULL);
    int v = queen_count(pos, NULL, NULL) == 0 ? 2 : 1;

    if (diagonal && bishop_xray_attack(pos, square, NULL))
        return v;
    if (!diagonal && rook_xray_attack(pos, square, NULL) && queen_attack(pos2, &(Square){square->x, 7 - square->y}, NULL))
        return v;

    return 0;
}

int knight_on_queen(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, knight_on_queen, NULL);
    }

    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    int qx = -1, qy = -1;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board(pos, x, y) == 'q')
            {
                if (qx >= 0 || qy >= 0)
                    return 0;
                qx = x;
                qy = y;
            }
        }
    }

    if (queen_count(pos2, NULL, NULL) != 1)
        return 0;
    if (board(pos, square->x, square->y) == 'P')
        return 0;
    if (board(pos, square->x - 1, square->y - 1) == 'p')
        return 0;
    if (board(pos, square->x + 1, square->y - 1) == 'p')
        return 0;
    if (attack(pos, square, NULL) <= 1 && attack(pos2, &(Square){square->x, 7 - square->y}, NULL) > 1)
        return 0;
    if (!mobility_area(pos, square, NULL))
        return 0;
    if (!knight_attack(pos, square, NULL))
        return 0;

    int v = queen_count(pos, NULL, NULL) == 0 ? 2 : 1;
    if (abs(qx - square->x) == 2 && abs(qy - square->y) == 1)
        return v;
    if (abs(qx - square->x) == 1 && abs(qy - square->y) == 2)
        return v;

    return 0;
}

int restricted(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, restricted, NULL);
    }

    if (attack(pos, square, NULL) == 0)
        return 0;

    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    if (!attack(pos2, &(Square){square->x, 7 - square->y}, NULL))
    {
        return 0;
    }

    if (pawn_attack(pos2, &(Square){square->x, 7 - square->y}, NULL) > 0)
    {
        return 0;
    }
    if (attack(pos2, &(Square){square->x, 7 - square->y}, NULL) > 1 && attack(pos, square, NULL) == 1)
    {
        return 0;
    }
    return 1;
}

int weak_queen_protection(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, weak_queen_protection, NULL);
    }

    if (!weak_enemies(pos, square, NULL))
        return 0;
    ChessPosition flipped;
    ChessPosition *pos2 = &flipped;
    colorflip(pos, pos2);
    if (!queen_attack(pos2, &(Square){square->x, 7 - square->y}, NULL))
        return 0;

    return 1;
}

int minor_threat_eval(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, minor_threat_eval, NULL);
    }

    int v = 0;
    int threat = minor_threat(pos, square, NULL);
    switch (threat)
    {
    case 0:
        v += 0;
        break;
    case 1:
        v += 5;
        break;
    case 2:
        v += 57;
        break;
    case 3:
        v += 77;
        break;
    case 4:
        v += 88;
        break;
    case 5:
        v += 79;
        break;
    default:
        v += 0;
        break;
    }
    return v;
}

int rook_threat_eval(ChessPosition *pos, Square *square, void *param)
{
    if (square == NULL)
    {
        return sum(pos, rook_threat_eval, NULL);
    }

    int v = 0;
    int threat = rook_threat(pos, square, NULL);
    switch (threat)
    {
    case 0:
        v += 0;
        break;
    case 1:
        v += 3;
        break;
    case 2:
        v += 37;
        break;
    case 3:
        v += 42;
        break;
    case 4:
        v += 0;
        break;
    case 5:
        v += 58;
        break;
    default:
        v += 0;
        break;
    }
    return v;
}

int threats_mg(ChessPosition *pos)
{
    int v = 0;

    pos->eval.hanging = hanging(pos, NULL, NULL);
    v += 69 * pos->eval.hanging;

    pos->eval.king_threat = king_threat(pos, NULL, NULL);
    v += pos->eval.king_threat > 0 ? 24 : 0;

    pos->eval.pawn_push_threat = pawn_push_threat(pos, NULL, NULL);
    v += 48 * pos->eval.pawn_push_threat;

    pos->eval.threat_safe_pawn = threat_safe_pawn(pos, NULL, NULL);
    v += 173 * pos->eval.threat_safe_pawn;

    pos->eval.slider_on_queen = slider_on_queen(pos, NULL, NULL);
    v += 60 * pos->eval.slider_on_queen;

    pos->eval.knight_on_queen = knight_on_queen(pos, NULL, NULL);
    v += 16 * pos->eval.knight_on_queen;

    pos->eval.restricted = restricted(pos, NULL, NULL);
    v += 7 * pos->eval.restricted;

    pos->eval.weak_queen_protection = weak_queen_protection(pos, NULL, NULL);
    v += 14 * pos->eval.weak_queen_protection;

    pos->eval.minor_threat = minor_threat_eval(pos, NULL, NULL);
    v += pos->eval.minor_threat;

    pos->eval.root_threat = rook_threat_eval(pos, NULL, NULL);
    v += pos->eval.root_threat;
    return v;
}

int threats_eg(ChessPosition *pos)
{
    int v = 0;

    v += 36 * hanging(pos, NULL, NULL);
    v += king_threat(pos, NULL, NULL) > 0 ? 89 : 0;
    v += 39 * pawn_push_threat(pos, NULL, NULL);
    v += 94 * threat_safe_pawn(pos, NULL, NULL);
    v += 18 * slider_on_queen(pos, NULL, NULL);
    v += 11 * knight_on_queen(pos, NULL, NULL);
    v += 7 * restricted(pos, NULL, NULL);

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            Square s = {x, y};

            // Process minor threat values
            switch (minor_threat(pos, &s, NULL))
            {
            case 0:
                v += 0;
                break;
            case 1:
                v += 32;
                break;
            case 2:
                v += 41;
                break;
            case 3:
                v += 56;
                break;
            case 4:
                v += 119;
                break;
            case 5:
                v += 161;
                break;
            default:
                v += 0;
                break;
            }

            // Process rook threat values
            switch (rook_threat(pos, &s, NULL))
            {
            case 0:
                v += 0;
                break;
            case 1:
                v += 46;
                break;
            case 2:
                v += 68;
                break;
            case 3:
                v += 60;
                break;
            case 4:
                v += 38;
                break;
            case 5:
                v += 41;
                break;
            default:
                v += 0;
                break;
            }
        }
    }

    return v;
}
