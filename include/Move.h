#pragma once

#include <string>
#include "Board.h"

struct Move {
    int startSquare;
    int endSquare;

    Move(const int &startSquare, const int &endSquare) {
        this->startSquare = startSquare;
        this->endSquare = endSquare;
    }

    explicit Move(std::string &move) {
        int startFile = (move[0] - 'a');
        int startRank = (move[1] - '1');

        int endFile = (move[2] - 'a');
        int endRank = (move[3] - '1');

        this->startSquare = startRank * 7 + startFile;
        this->endSquare = endRank * 7 + endFile;
    }

    bool operator==(const Move &other) const {
        return startSquare == other.startSquare && endSquare == other.endSquare;
    }
};
