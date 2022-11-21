#pragma once

#include "Board.h"

class Evaluator {
public:
    explicit Evaluator(Board *&board);

    virtual ~Evaluator() = 0;

    virtual double evaluate() = 0;

protected:
    const int MAX_EVAL = (int) 1e6;

    const static std::map<int, int> pieceValues;

    Board *board;
};
