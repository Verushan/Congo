#pragma once

#include "MoveGenerator.h"

class LionMoveGenerator : public MoveGenerator {
public:
    void generateMoves(std::forward_list<Move> &moves, Board &board) override;

    ~LionMoveGenerator() override;

protected:
private:
    static bool openLineToOpponentLion(Board &board, int &playerLion, int &opponentLion);

    static bool isValidLionMove(int &index);
};
