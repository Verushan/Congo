#pragma once

#include "MoveGenerator.h"
#include "Piece.h"
#include "Move.h"
#include <vector>

class ZebraMoveGenerator : public MoveGenerator {
public:
    ZebraMoveGenerator();

    void generateMoves(std::forward_list<Move> &moves, Board &board) override;

    ~ZebraMoveGenerator() override;

protected:
    const static int ZEBRA_MOVE_COUNT = 8;
    const static int ZEBRA_MOVES[ZEBRA_MOVE_COUNT];
};
