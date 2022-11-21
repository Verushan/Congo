#include "LionMoveGenerator.h"
#include "Board.h"

bool LionMoveGenerator::openLineToOpponentLion(Board &board, int &playerLion, int &opponentLion) {
    int startPosition = std::min(playerLion, opponentLion);
    int endPosition = std::max(playerLion, opponentLion);

    bool straightLine = true;
    int position = startPosition;

    for (position += Board::BOARD_DIM; position < endPosition && straightLine; position += Board::BOARD_DIM)
        straightLine &= (board.board[position] == None);

    if (straightLine && position == endPosition)
        return true;

    int diagIncrement = 8;
    int antiDiagIncrement = 6;

    int diagPosition = startPosition + diagIncrement;
    int antiDiagPosition = startPosition + antiDiagIncrement;

    bool diag = true, antiDiag = true;

    diag &= (board.board[diagPosition] == None);
    diagPosition += diagIncrement;

    antiDiag &= (board.board[antiDiagPosition] == None);
    antiDiagPosition += antiDiagIncrement;

    if (diag && diagPosition == endPosition)
        return true;

    if (antiDiag && antiDiagPosition == endPosition)
        return true;

    return false;
}

bool LionMoveGenerator::isValidLionMove(int &index) {
    int file = index % Board::BOARD_DIM;
    int rank = index / Board::BOARD_DIM;

    return rank != 3 && 2 <= file && file <= 4;
}

void LionMoveGenerator::generateMoves(std::forward_list<Move> &moves, Board &board) {
    int player = Lion | board.sideToPlay;
    int opponent = Lion | (board.sideToPlay ^ PIECE_COLOUR_MASK);

    int playerLion = board.piecePositions[player];
    int opponentLion = board.piecePositions[opponent];

    if (openLineToOpponentLion(board, playerLion, opponentLion)) {
        moves.push_front(Move(playerLion, opponentLion));
        ++mobilityScore;
        attackScore += LION_ATTACK_SCORE;
    }

    int currPosition = playerLion;
    int piece, positionColor;

    for (int moveOffset: SINGLE_CELL_MOVES) {
        currPosition += moveOffset;
        piece = board.board[currPosition];

        if (Board::isInvalidMove(currPosition)) {
            currPosition -= moveOffset;
            continue;
        }

        positionColor = piece & PIECE_COLOUR_MASK;

        if (positionColor != board.sideToPlay) {
            if (isValidLionMove(currPosition)) {
                moves.emplace_front(Move(playerLion, currPosition));
                ++mobilityScore;
                attackScore += (piece != None);
            }
        }

        currPosition -= moveOffset;
    }
}

LionMoveGenerator::~LionMoveGenerator() = default;
