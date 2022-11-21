#include "Evaluator.h"

#pragma once

class AdvancedEvaluator : public Evaluator {
public:
    double evaluate(Board *&board) override;
};
