#include "GiraffeMoveGenerator.h"

const int GiraffeMoveGenerator::JUMP_MOVES[MAX_GIRAFFE_JUMPS] = {-16, -14, -12, 2, 16, 14, 12, -2};

std::vector<int> GiraffeMoveGenerator::LEGAL_NAVIGATIONS[Board::BOARD_LENGTH];
std::vector<int> GiraffeMoveGenerator::LEGAL_CAPTURES[Board::BOARD_LENGTH];

GiraffeMoveGenerator::GiraffeMoveGenerator() {
    precomputeLegalMoves(JUMP_MOVES, MAX_GIRAFFE_JUMPS, 2);
    moveElements(LEGAL_MOVES, LEGAL_CAPTURES);

    precomputeLegalMoves(SINGLE_CELL_MOVES, SINGLE_CELL_MOVE_COUNT, 1);
    precomputeLegalMoves(JUMP_MOVES, MAX_GIRAFFE_JUMPS, 2);
    moveElements(LEGAL_MOVES, LEGAL_NAVIGATIONS);
}

void GiraffeMoveGenerator::generateMoves(std::forward_list<Move> &moves, Board &board) {
    int piece = Giraffe | board.sideToPlay;

    if (board.piecePositions[piece] == Board::OFF_BOARD)
        return;

    generateCaptures(moves, board, piece);
    generateNavigations(moves, board, piece);
}

void GiraffeMoveGenerator::generateCaptures(std::forward_list<Move> &moves, Board &board, int &piece) {
    int opponentColour = board.sideToPlay ^ PIECE_COLOUR_MASK;
    int position = board.piecePositions[piece];

    if (board.piecePositions[piece] == Board::OFF_BOARD)
        return;

    int positionColour, pieceType;

    for (int &move: LEGAL_CAPTURES[position]) {
        positionColour = board.board[move] & PIECE_COLOUR_MASK;

        if (positionColour == opponentColour) {
            moves.emplace_front(Move(position, move));
            ++mobilityScore;
            ++attackScore;

            pieceType = board.board[move] & PIECE_TYPE_MASK;

            if (pieceType == Lion)
                attackScore += LION_ATTACK_SCORE;
        }
    }
}

void GiraffeMoveGenerator::generateNavigations(std::forward_list<Move> &moves, Board &board, int &piece) {
    int position = board.piecePositions[piece];

    if (position == Board::OFF_BOARD)
        return;

    for (int &move: LEGAL_NAVIGATIONS[position]) {
        if (board.board[move] == None) {
            moves.emplace_front(Move(position, move));
            ++mobilityScore;
        }
    }
}

GiraffeMoveGenerator::~GiraffeMoveGenerator() = default;
