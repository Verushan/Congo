#pragma once

#include "MoveGenerator.h"
#include <vector>

class PawnMoveGenerator : public MoveGenerator {
public:
    PawnMoveGenerator();

    void generateMoves(std::forward_list<Move> &moves, Board &board) override;

    ~PawnMoveGenerator() override;

protected:
    std::vector<int> WHITE_FORWARD_MOVES[Board::BOARD_LENGTH];
    std::vector<int> BLACK_FORWARD_MOVES[Board::BOARD_LENGTH];

    const static int PAWN_FORWARD_MOVE_COUNT = 3;
    const static int PAWN_FORWARD_MOVES[2 * PAWN_FORWARD_MOVE_COUNT];

    const static int PAWN_BACKWARD_MOVE_COUNT = 2;
    const static int PAWN_BACKWARD_MOVES[2 * PAWN_BACKWARD_MOVE_COUNT];

    void generateForwardMoves(std::forward_list<Move> &moves, Board &board, int &position);

    static void generateBackwardMoves(std::forward_list<Move> &moves, Board &board, int &position);
};
