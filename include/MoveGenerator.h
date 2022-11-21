#pragma once

#include "Board.h"
#include "Move.h"
#include <forward_list>
#include <string>

class MoveGenerator {
public:
    virtual void generateMoves(std::forward_list<Move> &moves, Board &board) = 0;

    static int attackScore;
    static int mobilityScore;

    static void resetScores();

    virtual ~MoveGenerator() = 0;

protected:
    const static int SINGLE_CELL_MOVE_COUNT = 8;
    const static int SINGLE_CELL_MOVES[SINGLE_CELL_MOVE_COUNT];
    const static int LION_ATTACK_SCORE;

    std::vector<int> LEGAL_MOVES[Board::BOARD_LENGTH];

    void precomputeLegalMoves(const int *MOVES, int MOVE_COUNT, size_t DISTANCE);

    static void moveElements(std::vector<int> v1[Board::BOARD_LENGTH], std::vector<int> v2[Board::BOARD_LENGTH]);
};
