#include "AdvancedEvaluator.h"

AdvancedEvaluator::AdvancedEvaluator(Board *&board) : Evaluator(board) {
}

int AdvancedEvaluator::getMaterialScore() {
    int blackValue = 0;
    int whiteValue = 0;
    int pieceType, pieceColour;

    for (const int &piece: board->board) {
        if (piece == None)
            continue;

        pieceType = (piece & PIECE_TYPE_MASK);

        if (pieceType == Lion)
            continue;

        pieceColour = piece & PIECE_COLOUR_MASK;

        if (pieceColour == White)
            whiteValue += pieceValues.at(pieceType);
        else
            blackValue += pieceValues.at(pieceType);
    }

    return whiteValue - blackValue;
}

int AdvancedEvaluator::getMobilityScore(std::forward_list<Move> &whiteMoves, std::forward_list<Move> &blackMoves) {
    int whiteMoveCount = (int) std::distance(whiteMoves.begin(), whiteMoves.end());
    int blackMoveCount = (int) std::distance(blackMoves.begin(), blackMoves.end());
    return whiteMoveCount - blackMoveCount;
}

int AdvancedEvaluator::getAttackScore(std::forward_list<Move> &moves) {
    int score = 0;
    int opponentColour = board->sideToPlay ^ PIECE_COLOUR_MASK;
    int piece, pieceColour, pieceType;

    for (const Move &move: moves) {
        if (board->board[move.endSquare] == None)
            continue;

        piece = board->board[move.endSquare];

        pieceColour = piece & PIECE_COLOUR_MASK;
        pieceType = piece & PIECE_TYPE_MASK;

        // If this is move attacks the opponent's piece
        if (pieceColour == opponentColour) {
            ++score;

            if (pieceType == Lion)
                score += LION_ATTACK_SCORE;
        }
    }

    return score;
}

double AdvancedEvaluator::evaluate() {
    // Win
    if (board->isTerminalState()) {
        if (board->piecePositions[Lion | board->sideToPlay] != Board::OFF_BOARD)
            return MAX_EVAL;

        return -MAX_EVAL;
    }

    // Draw
    if (board->whitePieceCount == 1 && board->blackPieceCount == 1)
        return 0;

    std::forward_list<Move> whiteMoves, blackMoves;

    int whiteAttackScore = 0, blackAttackScore = 0;
    int whiteMobilityScore = 0, blackMobilityScore = 0;
    int pieceColour, pieceType;

    if (board->sideToPlay == White) {
        whiteMoves = board->generateMoves();
        board->flipSidesToPlay();
        blackMoves = board->generateMoves();
        board->flipSidesToPlay();
    } else {
        blackMoves = board->generateMoves();
        board->flipSidesToPlay();
        whiteMoves = board->generateMoves();
        board->flipSidesToPlay();
    }

    for (Move &move: whiteMoves) {
        ++whiteMobilityScore;

        if (board->board[move.endSquare] == None)
            continue;

        pieceColour = board->board[move.endSquare] & PIECE_COLOUR_MASK;
        pieceType = board->board[move.endSquare] & PIECE_TYPE_MASK;

        if (pieceColour == Black) {
            ++whiteAttackScore;

            if (pieceType == Lion)
                whiteAttackScore += LION_ATTACK_SCORE;
        }
    }

    for (Move &move: blackMoves) {
        ++blackMobilityScore;

        if (board->board[move.endSquare] == None)
            continue;

        pieceColour = board->board[move.endSquare] & PIECE_COLOUR_MASK;
        pieceType = board->board[move.endSquare] & PIECE_TYPE_MASK;

        if (pieceColour == White) {
            ++blackAttackScore;

            if (pieceType == Lion)
                blackAttackScore += LION_ATTACK_SCORE;
        }
    }

    int attackScore = whiteAttackScore - blackAttackScore;
    int materialValue = getMaterialScore();
    int mobilityScore = whiteMobilityScore - blackMobilityScore;

    return (materialValue + mobilityScore + attackScore) * (board->sideToPlay == White ? 1 : -1);
}
