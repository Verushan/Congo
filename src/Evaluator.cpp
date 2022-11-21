#include "Evaluator.h"

const std::map<int, int> Evaluator::pieceValues = {{Pawn,      100},
                                                   {Zebra,     300},
                                                   {SuperPawn, 350},
                                                   {Giraffe,   400}};

Evaluator::Evaluator(Board *&board) {
    this->board = board;
}

Evaluator::~Evaluator() = default;
