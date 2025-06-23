#include "header/chess/combination.h"

Move *queen_moves(ChessPosition *pos, Square *square, void *param, int *move_count)
{
    Square *s2 = (Square *)param; // Specific queen's position
    *move_count = 0;              // Initialize move count

    char currentQueen = pos->w ? 'Q' : 'q'; // Determine if white or black queen
    char targetPiece = board(pos, square->x, square->y);

    // Check if the current square has the correct queen for the current turn
    // and matches the specific square (if provided)
    if (targetPiece != currentQueen || (s2 && (s2->x != square->x || s2->y != square->y)))
    {
        return NULL;
    }

    // Allocate memory for move list (initial capacity of 64, can be resized if needed)
    int capacity = 64;
    Move *move_list = (Move *)malloc(capacity * sizeof(Move));

    for (int i = 0; i < 8; i++)
    {
        int ix = (i + (i > 3)) % 3 - 1;
        int iy = ((i + (i > 3)) / 3) - 1;

        for (int d = 1; d < 8; d++)
        {
            int newX = square->x + d * ix;
            int newY = square->y + d * iy;

            // Ensure the new position is within bounds
            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
            {
                break;
            }

            char pieceAtTarget = board(pos, newX, newY);

            // If there's a piece of the same side, break
            if ((pieceAtTarget >= 'A' && pieceAtTarget <= 'Z' && pos->w) ||
                (pieceAtTarget >= 'a' && pieceAtTarget <= 'z' && !pos->w))
            {
                break;
            }

            // Legal move found
            Move movement;
            movement.before.x = square->x;
            movement.before.y = square->y;
            movement.after.x = newX;
            movement.after.y = newY;

            // Append move to move_list
            if (*move_count >= capacity)
            {
                // Resize the move_list if needed
                capacity *= 2;
                move_list = (Move *)realloc(move_list, capacity * sizeof(Move));
            }
            move_list[*move_count] = movement;
            (*move_count)++;

            // Stop if there's a capture
            if (pieceAtTarget != '-')
            {
                break;
            }
        }
    }

    return move_list;
}

Move *knight_moves(ChessPosition *pos, Square *square, void *param, int *moveCount)
{

    Square *s2 = (Square *)param; // Specific square for the knight
    *moveCount = 0;

    char currentKnight = pos->w ? 'N' : 'n'; // Determine if white or black knight
    char targetPiece = board(pos, square->x, square->y);

    // Check if the current square has the correct knight for the current turn
    if (targetPiece != currentKnight || (s2 && (s2->x != square->x || s2->y != square->y)))
    {
        return NULL;
    }

    // Knight move offsets
    int knightOffsets[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    // Allocate initial memory for moves
    int maxMoves = 8; // Maximum possible moves for a knight
    Move *moveList = malloc(maxMoves * sizeof(Move));

    for (int i = 0; i < 8; i++)
    {
        int newX = square->x + knightOffsets[i][0];
        int newY = square->y + knightOffsets[i][1];

        // Ensure the new position is within bounds
        if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
        {
            continue;
        }

        char pieceAtTarget = board(pos, newX, newY);

        // If there's a piece of the same side, skip this move
        if ((pieceAtTarget >= 'A' && pieceAtTarget <= 'Z' && pos->w) ||
            (pieceAtTarget >= 'a' && pieceAtTarget <= 'z' && !pos->w))
        {
            continue;
        }

        // Add the legal move to the list
        moveList[*moveCount].before.x = square->x;
        moveList[*moveCount].before.y = square->y;
        moveList[*moveCount].after.x = newX;
        moveList[*moveCount].after.y = newY;

        (*moveCount)++;
    }

    // Resize the move list to fit the exact number of moves
    if (*moveCount > 0)
    {
        moveList = realloc(moveList, (*moveCount) * sizeof(Move));
    }
    else
    {
        free(moveList);
        moveList = NULL;
    }

    return moveList;
}

Move *bishop_moves(ChessPosition *pos, Square *square, void *param, int *moveCount)
{

    Square *s2 = (Square *)param; // Specific square for the bishop
    *moveCount = 0;

    char currentBishop = pos->w ? 'B' : 'b'; // Determine if white or black bishop
    char targetPiece = board(pos, square->x, square->y);

    // Check if the current square has the correct bishop for the current turn
    if (targetPiece != currentBishop || (s2 && (s2->x != square->x || s2->y != square->y)))
    {
        return NULL;
    }

    // Allocate memory for the moves dynamically (maximum 13 possible moves for a bishop)
    Move *moveList = malloc(13 * sizeof(Move));
    if (!moveList)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Iterate over diagonal directions
    for (int i = 0; i < 4; i++)
    {
        int ix = ((i > 1) * 2 - 1);      // Direction in x-axis
        int iy = ((i % 2 == 0) * 2 - 1); // Direction in y-axis

        for (int d = 1; d < 8; d++)
        {
            int newX = square->x + d * ix;
            int newY = square->y + d * iy;

            // Ensure the new position is within bounds
            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
            {
                break;
            }

            char pieceAtTarget = board(pos, newX, newY);

            // Stop if there's a piece of the same side
            if ((pieceAtTarget >= 'A' && pieceAtTarget <= 'Z' && pos->w) ||
                (pieceAtTarget >= 'a' && pieceAtTarget <= 'z' && !pos->w))
            {
                break;
            }

            // Add the legal move to the list
            moveList[*moveCount].before.x = square->x;
            moveList[*moveCount].before.y = square->y;
            moveList[*moveCount].after.x = newX;
            moveList[*moveCount].after.y = newY;
            (*moveCount)++;

            // Stop if there's a capture
            if (pieceAtTarget != '-')
            {
                break;
            }
        }
    }

    // Resize the move list to the actual number of moves
    moveList = realloc(moveList, (*moveCount) * sizeof(Move));
    if (!moveList && *moveCount > 0)
    {
        fprintf(stderr, "Memory reallocation failed\n");
        return NULL;
    }

    return moveList;
}

Move *rook_moves(ChessPosition *pos, Square *square, void *param, int *moveCount)
{
    if (!square)
    {
        *moveCount = 0;
        return NULL;
    }

    Square *s2 = (Square *)param; // Specific square for the rook
    *moveCount = 0;

    char currentRook = pos->w ? 'R' : 'r'; // Determine if white or black rook
    char targetPiece = board(pos, square->x, square->y);

    // Check if the current square has the correct rook for the current turn
    if (targetPiece != currentRook || (s2 && (s2->x != square->x || s2->y != square->y)))
    {
        return NULL;
    }

    // Allocate memory for the maximum possible moves (14 for a rook in an open board)
    Move *moveList = (Move *)malloc(14 * sizeof(Move));

    // Iterate over straight directions
    for (int i = 0; i < 4; i++)
    {
        int ix = (i == 0 ? -1 : i == 1 ? 1
                                       : 0); // Horizontal movement
        int iy = (i == 2 ? -1 : i == 3 ? 1
                                       : 0); // Vertical movement

        for (int d = 1; d < 8; d++)
        {
            int newX = square->x + d * ix;
            int newY = square->y + d * iy;

            // Ensure the new position is within bounds
            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
                break;

            char pieceAtTarget = board(pos, newX, newY);

            // Stop if there's a piece of the same side
            if ((pieceAtTarget >= 'A' && pieceAtTarget <= 'Z' && pos->w) ||
                (pieceAtTarget >= 'a' && pieceAtTarget <= 'z' && !pos->w))
            {
                break;
            }

            // Add the legal move to the move list
            moveList[*moveCount].before.x = square->x;
            moveList[*moveCount].before.y = square->y;
            moveList[*moveCount].after.x = newX;
            moveList[*moveCount].after.y = newY;
            (*moveCount)++;

            // Stop if there's a capture
            if (pieceAtTarget != '-')
                break;
        }
    }

    // Resize the move list to the actual number of moves
    moveList = (Move *)realloc(moveList, (*moveCount) * sizeof(Move));

    return moveList;
}

Move *pawn_moves(ChessPosition *pos, Square *square, void *param, int *moveCount)
{
    if (!square)
    {
        *moveCount = 0;
        return NULL;
    }

    Square *s2 = (Square *)param; // Specific pawn
    *moveCount = 0;

    char currentPawn = pos->w ? 'P' : 'p'; // Determine if white or black pawn
    char targetPiece = board(pos, square->x, square->y);

    // Check if the current square has the correct pawn for the current turn
    if (targetPiece != currentPawn || (s2 && (s2->x != square->x || s2->y != square->y)))
    {
        return NULL;
    }

    // Allocate memory for the maximum possible moves (4 for a pawn)
    Move *moveList = (Move *)malloc(4 * sizeof(Move));

    // Determine direction based on color (white moves up, black moves down)
    int forward = pos->w ? -1 : 1;

    // Check for forward move
    if (square->y + forward >= 0 && square->y + forward < 8 && board(pos, square->x, square->y + forward) == '-')
    {
        moveList[*moveCount].before.x = square->x;
        moveList[*moveCount].before.y = square->y;
        moveList[*moveCount].after.x = square->x;
        moveList[*moveCount].after.y = square->y + forward;
        (*moveCount)++;

        // Check for double move on starting rank
        int startingRank = pos->w ? 6 : 1;
        if (square->y == startingRank && board(pos, square->x, square->y + 2 * forward) == '-' &&
            square->y + 2 * forward >= 0 && square->y + 2 * forward < 8)
        {
            moveList[*moveCount].before.x = square->x;
            moveList[*moveCount].before.y = square->y;
            moveList[*moveCount].after.x = square->x;
            moveList[*moveCount].after.y = square->y + 2 * forward;
            (*moveCount)++;
        }
    }

    // Check for captures
    if (square->x - 1 >= 0 && square->y + forward >= 0 && square->y + forward < 8 &&
        ((pos->w && board(pos, square->x - 1, square->y + forward) >= 'a' && board(pos, square->x - 1, square->y + forward) <= 'z') ||
         (!pos->w && board(pos, square->x - 1, square->y + forward) >= 'A' && board(pos, square->x - 1, square->y + forward) <= 'Z')))
    {
        moveList[*moveCount].before.x = square->x;
        moveList[*moveCount].before.y = square->y;
        moveList[*moveCount].after.x = square->x - 1;
        moveList[*moveCount].after.y = square->y + forward;
        (*moveCount)++;
    }

    if (square->x + 1 < 8 && square->y + forward >= 0 && square->y + forward < 8 &&
        ((pos->w && board(pos, square->x + 1, square->y + forward) >= 'a' && board(pos, square->x + 1, square->y + forward) <= 'z') ||
         (!pos->w && board(pos, square->x + 1, square->y + forward) >= 'A' && board(pos, square->x + 1, square->y + forward) <= 'Z')))
    {
        moveList[*moveCount].before.x = square->x;
        moveList[*moveCount].before.y = square->y;
        moveList[*moveCount].after.x = square->x + 1;
        moveList[*moveCount].after.y = square->y + forward;
        (*moveCount)++;
    }

    // Check for en passant
    if (pos->e[0] != -1 && pos->e[1] != -1) // If an en passant square exists
    {
        int enPassantX = pos->e[0];
        int enPassantY = pos->e[1];

        // Check if the pawn can capture en passant
        if (square->y == (pos->w ? 3 : 4) && square->y + forward == enPassantY &&
            (square->x - 1 == enPassantX || square->x + 1 == enPassantX))
        {
            moveList[*moveCount].before.x = square->x;
            moveList[*moveCount].before.y = square->y;
            moveList[*moveCount].after.x = enPassantX;
            moveList[*moveCount].after.y = enPassantY;
            (*moveCount)++;
        }
    }

    // Resize the move list to the actual number of moves
    moveList = (Move *)realloc(moveList, (*moveCount) * sizeof(Move));

    return moveList;
}

Move *king_moves(ChessPosition *pos, Square *square, void *param, int *moveCount)
{
    if (!square)
    {
        *moveCount = 0;
        return NULL;
    }

    Square *s2 = (Square *)param; // Specific king square
    *moveCount = 0;

    char currentKing = pos->w ? 'K' : 'k'; // Determine if white or black king
    char targetPiece = board(pos, square->x, square->y);

    // Check if the current square has the correct king for the current turn
    if (targetPiece != currentKing || (s2 && (s2->x != square->x || s2->y != square->y)))
    {
        return NULL;
    }

    // Allocate memory for the maximum possible moves (8 for a king)
    Move *moveList = (Move *)malloc(8 * sizeof(Move));

    // Loop through all possible king moves (8 surrounding squares)
    for (int i = 0; i < 8; i++)
    {
        int ix = (i + (i > 3)) % 3 - 1;
        int iy = ((i + (i > 3)) / 3) - 1;
        if (ix == 0 && iy == 0)
            continue;
        int newX = square->x + ix;
        int newY = square->y + iy;

        // Ensure the new position is within bounds
        if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
            continue;

        char pieceAtTarget = board(pos, newX, newY);

        // Skip if the target square has a piece of the same side
        if ((pieceAtTarget >= 'A' && pieceAtTarget <= 'Z' && pos->w) ||
            (pieceAtTarget >= 'a' && pieceAtTarget <= 'z' && !pos->w))
            continue;

        if (!is_square_attacked(pos, newX, newY, !pos->w))
        {
            moveList[*moveCount].before.x = square->x;
            moveList[*moveCount].before.y = square->y;
            moveList[*moveCount].after.x = newX;
            moveList[*moveCount].after.y = newY;
            (*moveCount)++;
        }
    }

    // Resize the move list to the actual number of moves
    moveList = (Move *)realloc(moveList, (*moveCount) * sizeof(Move));

    return moveList;
}

Move *combination(ChessPosition *pos, int *totalMoves)
{
    Move *combinedList = NULL;
    *totalMoves = 0;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            char piece = board(pos, x, y);

            // Check if the piece belongs to the current turn
            if ((pos->w && piece >= 'A' && piece <= 'Z') || // White's turn
                (!pos->w && piece >= 'a' && piece <= 'z'))  // Black's turn
            {
                Square square = {x, y};
                int moveCount = 0;
                Move *pieceMoves = NULL;

                // Dispatch to the appropriate move function based on the piece type
                switch (piece)
                {
                case 'P':
                case 'p':
                    pieceMoves = pawn_moves(pos, &square, NULL, &moveCount);
                    break;
                case 'R':
                case 'r':
                    pieceMoves = rook_moves(pos, &square, NULL, &moveCount);
                    break;
                case 'N':
                case 'n':
                    pieceMoves = knight_moves(pos, &square, NULL, &moveCount);
                    break;
                case 'B':
                case 'b':
                    pieceMoves = bishop_moves(pos, &square, NULL, &moveCount);
                    break;
                case 'Q':
                case 'q':
                    pieceMoves = queen_moves(pos, &square, NULL, &moveCount);
                    break;
                case 'K':
                case 'k':
                    pieceMoves = king_moves(pos, &square, NULL, &moveCount);
                    break;
                default:
                    break; // Skip empty squares or invalid pieces
                }

                if (pieceMoves)
                {
                    // Allocate memory for the combined list
                    combinedList = (Move *)realloc(combinedList, (*totalMoves + moveCount) * sizeof(Move));

                    // Copy the piece's moves into the combined list
                    memcpy(combinedList + *totalMoves, pieceMoves, moveCount * sizeof(Move));

                    // Update the total move count
                    *totalMoves += moveCount;

                    // Free the piece's move list
                    free(pieceMoves);
                }
            }
        }
    }

    return combinedList;
}
