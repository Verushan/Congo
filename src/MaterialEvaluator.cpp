#include "MaterialEvaluator.h"

MaterialEvaluator::MaterialEvaluator(Board *&board) : Evaluator(board) {
}

double MaterialEvaluator::evaluate() {
    // Win
    if (board->isTerminalState()) {
        if (board->piecePositions[Lion | board->sideToPlay] != Board::OFF_BOARD)
            return MAX_EVAL;

        return -MAX_EVAL;
    }

    // Draw
    if (board->whitePieceCount == 1 && board->blackPieceCount == 1)
        return 0;

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

    return (whiteValue - blackValue) * (board->sideToPlay == White ? 1 : -1);
}
