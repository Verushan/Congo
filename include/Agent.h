#pragma once

#include "Board.h"
#include "Evaluator.h"
#include "MyTimer.h"

class Agent {
public:
    static int numStatesExplored;

    const static int INITIAL_ALPHA = (int) -1e6;
    const static int INITIAL_BETA = (int) 1e6;

    explicit Agent(const std::string &fen);

    void updateBoard(std::string &newBoardFen);

    void makeMove(Move &move);

    void unmakeMove(Move &move, const int &movingPiece, const int &capturedPiece, std::vector<int> &riverPieces);

    void moveGenerationTest();

    void exploreMoves(int depth);

    int minimax(int depth, int alpha, int beta);

    Move getMove(int &timeLimit);

    ~Agent();

protected:
    Board *board;
    Evaluator *evaluator;
    MyTimer timer;
    Move bestMove = NO_MOVE;

    const static int MAX_EVAL = (int) 1e6;
    const static int MAX_DEPTH = 25;
    const static int MIN_TIME_LIMIT_IN_MS = 500;
    const static Move NO_MOVE;

    int timeRemainingInMs;
    bool lastDepthCompleted;
};
