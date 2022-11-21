#include "MoveGenerator.h"

int MoveGenerator::attackScore = 0;
int MoveGenerator::mobilityScore = 0;
const int MoveGenerator::LION_ATTACK_SCORE = 10;

const int MoveGenerator::SINGLE_CELL_MOVE_COUNT;
const int MoveGenerator::SINGLE_CELL_MOVES[SINGLE_CELL_MOVE_COUNT] = {-1, 1, -7, 7, 6, -6, 8, -8};

void MoveGenerator::moveElements(std::vector<int> *v1, std::vector<int> *v2) {
    for (size_t i = 0; i < Board::BOARD_LENGTH; ++i)
        v2[i] = std::move(v1[i]);
}

void MoveGenerator::precomputeLegalMoves(const int *MOVES, const int MOVE_COUNT, const size_t DISTANCE) {
    int posRank, posFile;
    int movePosition;
    int moveRank, moveFile;

    for (int position = 0; position < Board::BOARD_LENGTH; ++position) {
        LEGAL_MOVES[position].reserve(MOVE_COUNT);

        posRank = position / Board::BOARD_DIM;
        posFile = position % Board::BOARD_DIM;

        for (size_t i = 0; i < MOVE_COUNT; ++i) {
            movePosition = position + MOVES[i];

            if (Board::isInvalidMove(movePosition))
                continue;

            moveRank = movePosition / Board::BOARD_DIM;
            moveFile = movePosition % Board::BOARD_DIM;

            moveRank = abs(moveRank - posRank);
            moveFile = abs(moveFile - posFile);

            if (std::max(moveRank, moveFile) == DISTANCE)
                LEGAL_MOVES[position].push_back(movePosition);
        }
    }
}

void MoveGenerator::resetScores() {
    attackScore = 0;
    mobilityScore = 0;
}

MoveGenerator::~MoveGenerator() = default;
