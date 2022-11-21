#pragma once

#include "MoveGenerator.h"

class SuperPawnMoveGenerator : public MoveGenerator {
public:
    SuperPawnMoveGenerator();

    void generateMoves(std::forward_list<Move> &moves, Board &board) override;

    ~SuperPawnMoveGenerator() override = default;

protected:
    const static int NUM_CAPTURES = 5;
    const static int NUM_RETREATS = 6;

    const static int CAPTURE_MOVES[2 * NUM_CAPTURES];
    const static int WHITE_RETREAT_MOVES[NUM_RETREATS];
    const static int BLACK_RETREAT_MOVES[NUM_RETREATS];

    std::vector<int> WHITE_CAPTURES[Board::BOARD_LENGTH];
    std::vector<int> BLACK_CAPTURES[Board::BOARD_LENGTH];

    void generateCaptureMoves(std::forward_list<Move> &moves, Board &board, int &position);

    static void generateRetreatMoves(std::forward_list<Move> &moves, Board &board, int &position);

    static bool validateRetreatMove(Board &board, int &currentPosition, int &movePosition);
};
