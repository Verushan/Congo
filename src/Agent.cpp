#include "Agent.h"
#include "AdvancedEvaluator.h"
#include "MyTimer.h"

int Agent::numStatesExplored = 0;
const Move Agent::NO_MOVE = Move(-1, -1);

Agent::Agent(const std::string &fen) {
    board = new Board(fen);
    evaluator = new AdvancedEvaluator(board);
    lastDepthCompleted = true;
    timeRemainingInMs = MAX_EVAL;
}

void Agent::updateBoard(std::string &newBoardFen) {
    delete board;
    board = new Board(newBoardFen);
}

Move Agent::getMove(int &timeLimit) {
    lastDepthCompleted = true;
    timeRemainingInMs = timeLimit;

    Move currBestMove = NO_MOVE;
    timer.startTimer();

    std::forward_list<Move> validMoves = board->generateMoves();
    int bestScore = -MAX_EVAL;

    for (int currDepth = 1; currDepth <= MAX_DEPTH; ++currDepth) {
        bestScore = minimax(currDepth, INITIAL_ALPHA, INITIAL_BETA);

        if (!lastDepthCompleted) {
            return currBestMove;
        }

        currBestMove = bestMove;
        bestMove = NO_MOVE;
    }

    return currBestMove;
}

void Agent::moveGenerationTest() {
    const int DEPTH_LIMIT = 6;
    long long elapsed;
    int prev = 0, curr;

    for (int depth = 1; depth <= DEPTH_LIMIT; ++depth) {
        numStatesExplored = 0;
        timer.startTimer();
        exploreMoves(depth);
        elapsed = timer.getElapsedTime();
        curr = numStatesExplored;
        numStatesExplored -= prev;
        prev = curr;
        printf("Depth: %d ply Result: %d positions Time: %lld milliseconds\n", depth, numStatesExplored, elapsed);
    }
}

void Agent::exploreMoves(int depth) {
    if (board->isTerminalState() || depth == 0)
        return;

    int movingPiece, capturedPiece;
    std::vector<int> riverPieces(Board::BOARD_DIM);

    for (Move &move: board->generateMoves()) {
        ++numStatesExplored;
        std::copy(board->board + Board::RIVER_START_INDEX, board->board + Board::RIVER_END_INDEX, riverPieces.begin());

        movingPiece = board->board[move.startSquare];
        capturedPiece = board->board[move.endSquare];
        makeMove(move);
        exploreMoves(depth - 1);
        unmakeMove(move, movingPiece, capturedPiece, riverPieces);
    }
}

int Agent::minimax(int depth, int alpha, int beta) {
    if (timeRemainingInMs - timer.getElapsedTime() < MIN_TIME_LIMIT_IN_MS) {
        lastDepthCompleted = false;
        return beta;
    }

    if (board->isTerminalState() || depth <= 0)
        return (int) evaluator->evaluate();

    int bestScore = -MAX_EVAL, currScore;
    Move currBestMove = NO_MOVE;

    int movingPiece, capturedPiece;
    std::vector<int> riverPieces(Board::BOARD_DIM);

    for (Move &move: board->generateMoves()) {
        // Store the river before making the move
        std::copy(board->board + Board::RIVER_START_INDEX, board->board + Board::RIVER_END_INDEX, riverPieces.begin());

        movingPiece = board->board[move.startSquare];
        capturedPiece = board->board[move.endSquare];
        makeMove(move);

        currScore = -minimax(depth - 1, -beta, -alpha);
        bestScore = std::max(bestScore, currScore);

        unmakeMove(move, movingPiece, capturedPiece, riverPieces);

        if (bestScore >= beta)
            return beta;

        if (bestScore > alpha) {
            alpha = bestScore;
            currBestMove = move;
        }
    }

    bestMove = currBestMove;
    return bestScore;
}

void Agent::makeMove(Move &move) {
    // Storing piece beforehand to cater for making and unmaking promotion moves
    int pieceToMove = board->board[move.startSquare];

    // Piece won't drown
    if (!(Board::isInRiver(move.startSquare) && Board::isInRiver(move.endSquare))) {
        board->removeRiverPieces(move);
        board->makeMove(move);
    } else { // Make move then drown
        board->removeRiverPieces(move);
        board->makeMove(move);

        // Remove the moving piece
        board->updatePiecePosition(pieceToMove, move.endSquare, Board::OFF_BOARD);
    }

    // Update the move counter only if it's Black's turn
    if (board->sideToPlay == Black)
        ++board->moveCount;

    // Update the side to play
    board->sideToPlay ^= PIECE_COLOUR_MASK;
}

void Agent::unmakeMove(Move &move, const int &movingPiece, const int &capturedPiece, std::vector<int> &riverPieces) {
    // Update move counter only if it's now White's turn to play
    if (board->sideToPlay == White)
        --board->moveCount;

    // Revert the side to play
    board->sideToPlay ^= PIECE_COLOUR_MASK;

    int pieceColour, squareIndex;

    // Restore pieces that were previously on the river
    for (int i = 0; i < Board::BOARD_DIM; ++i) {
        squareIndex = Board::RIVER_START_INDEX + i;

        if (squareIndex == move.startSquare || squareIndex == move.endSquare)
            continue;

        pieceColour = riverPieces[i] & PIECE_COLOUR_MASK;

        if (pieceColour == board->sideToPlay)
            board->updatePiecePosition(riverPieces[i], Board::OFF_BOARD, Board::RIVER_START_INDEX + i);
    }

    // Cater for if a move was made that resulted in both the moving and captured pieces to drown
    if (Board::isInRiver(move.startSquare) && Board::isInRiver(move.endSquare))
        board->updatePiecePosition(movingPiece, Board::OFF_BOARD, move.endSquare);

    // Undo the original move
    board->unmakeMove(move, movingPiece, capturedPiece);
}

Agent::~Agent() {
    delete board;
    delete evaluator;
}
