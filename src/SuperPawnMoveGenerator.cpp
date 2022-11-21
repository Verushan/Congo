#include "SuperPawnMoveGenerator.h"

const int SuperPawnMoveGenerator::CAPTURE_MOVES[2 * NUM_CAPTURES] = {-1, 1, 6, 7, 8, 1, -1, -6, -7, -8};

const int SuperPawnMoveGenerator::WHITE_RETREAT_MOVES[NUM_RETREATS] = {-8, -7, -6, -16, -14, -12};

const int SuperPawnMoveGenerator::BLACK_RETREAT_MOVES[NUM_RETREATS] = {8, 7, 6, 16, 14, 12};

SuperPawnMoveGenerator::SuperPawnMoveGenerator() {
    precomputeLegalMoves(CAPTURE_MOVES, NUM_CAPTURES, 1);
    moveElements(LEGAL_MOVES, WHITE_CAPTURES);

    precomputeLegalMoves(CAPTURE_MOVES + NUM_CAPTURES, NUM_CAPTURES, 1);
    moveElements(LEGAL_MOVES, BLACK_CAPTURES);
}

void SuperPawnMoveGenerator::generateMoves(std::forward_list<Move> &moves, Board &board) {
    int startIndex = (board.sideToPlay == White ? 0 : Board::MINOR_PIECE_COUNT);
    int position, pieceType;

    for (int piece = startIndex; piece < startIndex + Board::MINOR_PIECE_COUNT; ++piece) {
        position = board.minorPiecePositions[piece];

        if (position == Board::OFF_BOARD)
            continue;

        pieceType = board.board[position] & PIECE_TYPE_MASK;

        if (pieceType != SuperPawn)
            continue;

        generateCaptureMoves(moves, board, position);
        generateRetreatMoves(moves, board, position);
    }
}

void SuperPawnMoveGenerator::generateCaptureMoves(std::forward_list<Move> &moves, Board &board, int &position) {
    std::vector<int> *availableMoves = (board.sideToPlay == White ? WHITE_CAPTURES : BLACK_CAPTURES);
    int piece, pieceColour, pieceType;
    int opponentColour = board.sideToPlay ^ PIECE_COLOUR_MASK;

    for (int &move: availableMoves[position]) {
        piece = board.board[move];
        pieceColour = piece & PIECE_COLOUR_MASK;

        if (pieceColour == opponentColour || piece == None) {
            moves.emplace_front(Move(position, move));
            ++mobilityScore;

            if (piece != None) {
                ++attackScore;
                pieceType = board.board[move] & PIECE_TYPE_MASK;

                if (pieceType == Lion)
                    attackScore += LION_ATTACK_SCORE;
            }
        }
    }
}

void SuperPawnMoveGenerator::generateRetreatMoves(std::forward_list<Move> &moves, Board &board, int &position) {
    const int *moveOffsets = (board.sideToPlay == White ? WHITE_RETREAT_MOVES : BLACK_RETREAT_MOVES);

    int currPosition = position;
    int len2RetreatPosition = position;
    int halfRetreatMoves = NUM_RETREATS / 2;

    for (int move = 0; move < halfRetreatMoves; ++move) {
        currPosition += moveOffsets[move];

        if (validateRetreatMove(board, position, currPosition)) {
            moves.emplace_front(Move(position, currPosition));
            ++mobilityScore;

            len2RetreatPosition += moveOffsets[halfRetreatMoves + move];

            if (validateRetreatMove(board, position, len2RetreatPosition)) {
                ++mobilityScore;
                moves.emplace_front(Move(position, len2RetreatPosition));
            }

            len2RetreatPosition -= moveOffsets[halfRetreatMoves + move];
        }

        currPosition -= moveOffsets[move];
    }
}

bool SuperPawnMoveGenerator::validateRetreatMove(Board &board, int &currentPosition, int &movePosition) {
    if (Board::isInvalidMove(movePosition))
        return false;

    if (board.board[movePosition] != None)
        return false;

    int currRank = currentPosition / Board::BOARD_DIM;
    int currFile = currentPosition % Board::BOARD_DIM;

    int otherRank = movePosition / Board::BOARD_DIM;
    int otherFile = movePosition % Board::BOARD_DIM;

    int dist = std::max(abs(currRank - otherRank), abs(currFile - otherFile));
    return dist <= 2;
}
