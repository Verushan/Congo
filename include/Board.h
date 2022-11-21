#pragma once

#include "Move.h"
#include "Piece.h"

#include <map>
#include <vector>
#include <string>
#include <array>
#include <forward_list>
#include <iostream>
#include <algorithm>
#include <sstream>

class MoveGenerator;

class Board {
public:
    const static std::string STARTING_FEN;
    const static int BOARD_DIM = 7;
    const static int OFF_BOARD = -1;
    const static int BOARD_LENGTH = BOARD_DIM * BOARD_DIM;
    const static int PIECE_POSITION_COUNT = 22;
    const static int MINOR_PIECE_COUNT = 8;
    const static int RIVER_START_INDEX = 21;
    const static int RIVER_END_INDEX = 28;
    const static int RIVER_RANK_INDEX = 3;

    int blackPieceCount, whitePieceCount;
    int board[BOARD_LENGTH]{};
    int piecePositions[PIECE_POSITION_COUNT]{};
    int minorPiecePositions[2 * MINOR_PIECE_COUNT]{};
    int moveCount;
    int sideToPlay;

    explicit Board(const std::string &fen);

    void printBoard();

    std::string getBoardAsFen();

    std::forward_list<Move> generateMoves();

    void printValidMoves();

    void makeMove(Move &move);

    void unmakeMove(Move &move, int movingPiece, int capturedPiece);

    void removeRiverPieces(Move &move);

    bool isTerminalState();

    void debugPrint();

    static bool isInRiver(int position);

    static bool isInvalidMove(int &index);

    static std::string indexToCoordinate(int &index);

    static std::string moveToString(Move move);

    void updatePiecePosition(const int &piece, const int &oldPosition, const int &newPosition);

    int getMinorPieceLocationIndex(const int &pieceColour, const int &initialPosition);

    void increasePieceCount(const int &pieceColour);

    void decreasePieceCount(const int &pieceColour);

    void flipSidesToPlay();

    ~Board();

private:
    MoveGenerator *lionMoveGenerator;
    MoveGenerator *zebraMoveGenerator;
    MoveGenerator *giraffeMoveGenerator;
    MoveGenerator *pawnMoveGenerator;
    MoveGenerator *superPawnMoveGenerator;

protected:
    const std::map<char, Piece> pieceTypeFromSymbol = {
            {'l', Lion},
            {'p', Pawn},
            {'s', SuperPawn},
            {'z', Zebra},
            {'g', Giraffe}};

    friend class MoveGenerator;

    friend class LionMoveGenerator;

    friend class ZebraMoveGenerator;

    friend class GiraffeMoveGenerator;

    friend class PawnMoveGenerator;

    friend class SuperPawnMoveGenerator;
};
