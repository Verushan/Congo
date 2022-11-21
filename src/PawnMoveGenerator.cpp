#include "PawnMoveGenerator.h"

const int PawnMoveGenerator::PAWN_FORWARD_MOVES[2 * PAWN_FORWARD_MOVE_COUNT] = {6, 7, 8, -6, -7, -8};

const int PawnMoveGenerator::PAWN_BACKWARD_MOVES[2 * PAWN_BACKWARD_MOVE_COUNT] = {-7, -14, 7, 14};

PawnMoveGenerator::PawnMoveGenerator() {
    precomputeLegalMoves(PAWN_FORWARD_MOVES, 3, 1);
    moveElements(LEGAL_MOVES, WHITE_FORWARD_MOVES);

    precomputeLegalMoves(PAWN_FORWARD_MOVES + 3, 3, 1);
    moveElements(LEGAL_MOVES, BLACK_FORWARD_MOVES);
}

void PawnMoveGenerator::generateMoves(std::forward_list<Move> &moves, Board &board) {
    int startIndex = (board.sideToPlay == White ? 0 : Board::MINOR_PIECE_COUNT);
    int piecePosition;

    for (int pawnIndex = startIndex; pawnIndex < startIndex + Board::MINOR_PIECE_COUNT; ++pawnIndex) {
        piecePosition = board.minorPiecePositions[pawnIndex];

        if (piecePosition == Board::OFF_BOARD)
            continue;

        // Checking for a super pawn
        if ((board.board[piecePosition] & PIECE_TYPE_MASK) != Pawn)
            continue;

        generateForwardMoves(moves, board, piecePosition);
        generateBackwardMoves(moves, board, piecePosition);
    }
}

void PawnMoveGenerator::generateForwardMoves(std::forward_list<Move> &moves, Board &board, int &position) {
    std::vector<int> *availableMoves = (board.sideToPlay == White ? WHITE_FORWARD_MOVES : BLACK_FORWARD_MOVES);
    int opponentColour = board.sideToPlay ^ PIECE_COLOUR_MASK;
    int pieceColour, pieceType;

    for (int &move: availableMoves[position]) {
        pieceColour = board.board[move] & PIECE_COLOUR_MASK;

        if (pieceColour == opponentColour || board.board[move] == None) {
            moves.emplace_front(Move(position, move));
            ++mobilityScore;

            pieceType = board.board[move] & PIECE_TYPE_MASK;

            if (pieceType == Lion)
                attackScore += LION_ATTACK_SCORE;
            else if (pieceType != None)
                ++attackScore;
        }
    }
}

void PawnMoveGenerator::generateBackwardMoves(std::forward_list<Move> &moves, Board &board, int &position) {
    int rank = position / Board::BOARD_DIM;

    if (board.sideToPlay == White && rank <= Board::RIVER_RANK_INDEX)
        return;

    if (board.sideToPlay == Black && rank >= Board::RIVER_RANK_INDEX)
        return;

    int startIndex = (board.sideToPlay == White ? 0 : PAWN_BACKWARD_MOVE_COUNT);
    int movePosition = position;

    for (int moveIndex = startIndex; moveIndex < startIndex + PAWN_BACKWARD_MOVE_COUNT; ++moveIndex) {
        movePosition += PAWN_BACKWARD_MOVES[moveIndex];

        if (board.board[movePosition] != None)
            return;

        moves.emplace_front(Move(position, movePosition));
        ++mobilityScore;
        movePosition -= PAWN_BACKWARD_MOVES[moveIndex];
    }
}

PawnMoveGenerator::~PawnMoveGenerator() = default;
