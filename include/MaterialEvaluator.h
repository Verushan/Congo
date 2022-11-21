#include "Evaluator.h"

#pragma once

class MaterialEvaluator : public Evaluator {
public:
    explicit MaterialEvaluator(Board *&board);

    double evaluate() override;
};
