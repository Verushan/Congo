#include "ZebraMoveGenerator.h"

const int ZebraMoveGenerator::ZEBRA_MOVES[ZEBRA_MOVE_COUNT] = {5, 13, 15, 9, -5, -13, -15, -9};

ZebraMoveGenerator::ZebraMoveGenerator() {
    precomputeLegalMoves(ZEBRA_MOVES, ZEBRA_MOVE_COUNT, 2);
}

void ZebraMoveGenerator::generateMoves(std::forward_list<Move> &moves, Board &board) {
    int playerZebra = Zebra | board.sideToPlay;

    int zebraPosition = board.piecePositions[playerZebra];

    if (zebraPosition == Board::OFF_BOARD)
        return;

    int piece, pieceType, positionColor;

    for (int &validMoves: LEGAL_MOVES[zebraPosition]) {
        piece = board.board[validMoves];
        positionColor = piece & PIECE_COLOUR_MASK;

        if (positionColor != board.sideToPlay) {
            ++mobilityScore;
            attackScore += (piece != None);

            moves.emplace_front(Move(zebraPosition, validMoves));

            pieceType = piece & PIECE_TYPE_MASK;

            if (pieceType == Lion)
                attackScore += LION_ATTACK_SCORE;
        }
    }
}

ZebraMoveGenerator::~ZebraMoveGenerator() = default;
