#pragma once

#include <vector>
#include "MoveGenerator.h"
#include "Board.h"

class GiraffeMoveGenerator : public MoveGenerator {
public:
    GiraffeMoveGenerator();

    void generateMoves(std::forward_list<Move> &moves, Board &board) override;

    ~GiraffeMoveGenerator() override;

protected:
    const static int MAX_GIRAFFE_JUMPS = 8;
    const static int JUMP_MOVES[MAX_GIRAFFE_JUMPS];

    static std::vector<int> LEGAL_NAVIGATIONS[Board::BOARD_LENGTH];
    static std::vector<int> LEGAL_CAPTURES[Board::BOARD_LENGTH];

    static void generateCaptures(std::forward_list<Move> &moves, Board &board, int &piece);

    static void generateNavigations(std::forward_list<Move> &moves, Board &board, int &piece);
};
