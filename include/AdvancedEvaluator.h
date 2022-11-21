#pragma once

#include "Evaluator.h"

class AdvancedEvaluator : public Evaluator {
public:
    explicit AdvancedEvaluator(Board *&board);

    int getMaterialScore();

    static int getMobilityScore(std::forward_list<Move> &whiteMoves, std::forward_list<Move> &blackMoves);

    int getAttackScore(std::forward_list<Move> &moves);

    double evaluate() override;

private:
    const int LION_ATTACK_SCORE = 10;
};