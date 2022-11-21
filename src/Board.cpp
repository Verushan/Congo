#include "Board.h"
#include "LionMoveGenerator.h"
#include "ZebraMoveGenerator.h"
#include "GiraffeMoveGenerator.h"
#include "PawnMoveGenerator.h"
#include "SuperPawnMoveGenerator.h"

const int Board::OFF_BOARD;
const std::string Board::STARTING_FEN = "g2l2z/ppppppp/7/7/7/PPPPPPP/G2L2Z w 0";

Board::Board(const std::string &fen) {
    std::fill_n(piecePositions, PIECE_POSITION_COUNT, OFF_BOARD);
    std::fill_n(minorPiecePositions, 2 * MINOR_PIECE_COUNT, OFF_BOARD);
    std::fill_n(board, BOARD_LENGTH, None);

    whitePieceCount = 0;
    blackPieceCount = 0;

    std::stringstream ss(fen);
    std::string fenBoard;

    getline(ss, fenBoard, ' ');

    std::string temp;
    getline(ss, temp, ' ');
    sideToPlay = (temp == "w" ? White : Black);

    getline(ss, temp, ' ');
    moveCount = stoi(temp);

    char piece;
    int pieceColor, pieceType, pieceValue, pieceLocation;
    int file = 0;
    int rank = BOARD_DIM - 1;
    int whitePawnCount = 0;
    int blackPawnCount = 0;

    for (char c: fenBoard) {
        if (c == '/') {
            file = 0;
            rank--;
            continue;
        }

        if ('1' <= c && c <= '7') {
            file += c - '0';
            continue;
        }

        piece = (char) tolower(c);
        pieceColor = (piece == c ? Black : White);

        if (pieceColor == White)
            ++whitePieceCount;
        else
            ++blackPieceCount;

        pieceType = pieceTypeFromSymbol.at(piece);
        pieceValue = pieceColor | pieceType;
        pieceLocation = rank * BOARD_DIM + file;

        // Treat pawns and super pawns equally
        if (pieceType == Pawn || pieceType == SuperPawn) {
            if (pieceColor == Black)
                minorPiecePositions[MINOR_PIECE_COUNT + blackPawnCount++] = pieceLocation;
            else
                minorPiecePositions[whitePawnCount++] = pieceLocation;
        } else
            piecePositions[pieceValue] = pieceLocation;

        board[pieceLocation] = pieceValue;
        ++file;
    }

    lionMoveGenerator = new LionMoveGenerator();
    zebraMoveGenerator = new ZebraMoveGenerator();
    giraffeMoveGenerator = new GiraffeMoveGenerator();
    pawnMoveGenerator = new PawnMoveGenerator();
    superPawnMoveGenerator = new SuperPawnMoveGenerator();
}

void Board::printBoard() {
    int index;

    for (int rank = BOARD_DIM - 1; rank >= 0; --rank) {
        printf("%d ", rank + 1);

        for (int file = 0; file < BOARD_DIM; ++file) {
            index = rank * BOARD_DIM + file;
            char symbol;
            int piece = board[index] & PIECE_TYPE_MASK;

            switch (piece) {
                case Pawn:
                    symbol = 'p';
                    break;
                case Lion:
                    symbol = 'l';
                    break;
                case Giraffe:
                    symbol = 'g';
                    break;
                case Zebra:
                    symbol = 'z';
                    break;
                case SuperPawn:
                    symbol = 's';
                    break;
                default:
                    symbol = '_';
            }

            if ((board[index] & PIECE_COLOUR_MASK) == White)
                symbol = (char) toupper(symbol);

            printf("%c  ", symbol);
        }

        printf("\n");
    }

    printf("  ");

    for (int i = 'a'; i <= 'g'; i++) {
        printf("%c  ", i);
    }

    printf("\n");
}

std::forward_list<Move> Board::generateMoves() {
    std::forward_list<Move> moves;
    lionMoveGenerator->generateMoves(moves, *this);
    zebraMoveGenerator->generateMoves(moves, *this);
    giraffeMoveGenerator->generateMoves(moves, *this);
    pawnMoveGenerator->generateMoves(moves, *this);
    superPawnMoveGenerator->generateMoves(moves, *this);
    return moves;
}

int Board::getMinorPieceLocationIndex(const int &pieceColour, const int &initialPosition) {
    int startIndex = (pieceColour == White ? 0 : MINOR_PIECE_COUNT);

    for (int i = startIndex; i < startIndex + MINOR_PIECE_COUNT; ++i) {
        if (minorPiecePositions[i] == initialPosition) {
            return i;
        }
    }

    throw std::runtime_error("Invalid piece position");
}

void Board::updatePiecePosition(const int &piece, const int &oldPosition, const int &newPosition) {
    int pieceType = piece & PIECE_TYPE_MASK;
    int pieceColour = piece & PIECE_COLOUR_MASK;
    int pieceLocationIndex;

    if (oldPosition != OFF_BOARD)
        board[oldPosition] = None;
    else
        increasePieceCount(pieceColour);

    if (pieceType == Pawn || pieceType == SuperPawn) {
        pieceLocationIndex = getMinorPieceLocationIndex(pieceColour, oldPosition);
        minorPiecePositions[pieceLocationIndex] = newPosition;
    } else {
        pieceLocationIndex = pieceColour | pieceType;
        piecePositions[pieceLocationIndex] = newPosition;
    }

    if (newPosition != OFF_BOARD) {
        if (pieceType == Pawn) { // Promotion
            int promotionRank = (sideToPlay == White ? BOARD_DIM - 1 : 0);
            int pawnNewRank = newPosition / BOARD_DIM;

            if (pawnNewRank == promotionRank)
                board[newPosition] = SuperPawn | sideToPlay;
            else
                board[newPosition] = piece;
        } else
            board[newPosition] = piece;
    } else
        decreasePieceCount(pieceColour);
}

bool Board::isInRiver(int position) {
    return RIVER_START_INDEX <= position && position < RIVER_END_INDEX;
}

void Board::increasePieceCount(const int &pieceColour) {
    if (pieceColour == White)
        ++whitePieceCount;
    else
        ++blackPieceCount;
}

void Board::decreasePieceCount(const int &pieceColour) {
    if (pieceColour == White)
        --whitePieceCount;
    else
        --blackPieceCount;
}

void Board::makeMove(Move &move) {
    int startSquarePiece = board[move.startSquare];
    int endSquarePiece = board[move.endSquare];

    if (endSquarePiece != None)
        updatePiecePosition(endSquarePiece, move.endSquare, OFF_BOARD);

    updatePiecePosition(startSquarePiece, move.startSquare, move.endSquare);
}

void Board::unmakeMove(Move &move, int movingPiece, int capturedPiece) {
    updatePiecePosition(movingPiece, move.endSquare, move.startSquare);

    if (capturedPiece != None)
        updatePiecePosition(capturedPiece, OFF_BOARD, move.endSquare);
}

void Board::removeRiverPieces(Move &move) {
    int piece, pieceColour;

    for (int i = RIVER_START_INDEX; i < RIVER_END_INDEX; ++i) {
        if (i == move.startSquare || i == move.endSquare)
            continue;

        piece = board[i];

        if (piece == None)
            continue;

        pieceColour = piece & PIECE_COLOUR_MASK;

        // Remove piece from the board
        if (sideToPlay == pieceColour)
            updatePiecePosition(piece, i, OFF_BOARD);
    }
}

std::string Board::getBoardAsFen() {
    static std::map<int, char> pieceSymbols = {
            {Zebra,     'z'},
            {Lion,      'l'},
            {Giraffe,   'g'},
            {Pawn,      'p'},
            {SuperPawn, 's'},
    };

    std::stringstream output;

    for (int rank = BOARD_DIM - 1; rank >= 0; --rank) {
        int emptyCount = 0;

        for (int file = 0; file < BOARD_DIM; ++file) {
            int piece = board[rank * BOARD_DIM + file];

            if (piece == None) {
                ++emptyCount;
            } else {
                int pieceType = piece & PIECE_TYPE_MASK;
                int pieceColour = piece & PIECE_COLOUR_MASK;

                char pieceSymbol = pieceSymbols[pieceType];
                pieceSymbol = (char) (pieceColour == White ? toupper(pieceSymbol) : pieceSymbol);

                if (emptyCount > 0) {
                    output << emptyCount;
                    emptyCount = 0;
                }

                output << pieceSymbol;
            }
        }

        if (emptyCount > 0)
            output << emptyCount;

        if (rank > 0)
            output << "/";
    }

    char playerColour = (sideToPlay == White ? 'w' : 'b');
    output << " " << playerColour << " " << moveCount << "\n";
    return output.str();
}

void Board::printValidMoves() {
    std::forward_list<Move> moves = generateMoves();

    long numMoves = std::distance(moves.begin(), moves.end());

    std::vector<std::string> sortedMoves;
    sortedMoves.reserve(numMoves);

    for (Move move: moves) {
        std::string strMove = moveToString(move);
        sortedMoves.emplace_back(strMove);
    }

    sort(sortedMoves.begin(), sortedMoves.end());

    if (numMoves == 0) {
        std::cout << "\n";
        return;
    }

    std::cout << sortedMoves[0];

    for (size_t i = 1; i < numMoves; ++i)
        std::cout << " " << sortedMoves[i];

    std::cout << "\n";
}

bool Board::isTerminalState() {
    return piecePositions[Lion | sideToPlay] == OFF_BOARD ||
           piecePositions[Lion | (sideToPlay ^ PIECE_COLOUR_MASK)] == OFF_BOARD;
}

std::string Board::indexToCoordinate(int &index) {
    char file = (char) ('a' + index % BOARD_DIM);
    int rank = index / BOARD_DIM;

    return file + std::to_string(rank + 1);
}

std::string Board::moveToString(Move move) {
    std::string moveStr;
    moveStr += indexToCoordinate(move.startSquare);
    moveStr += indexToCoordinate(move.endSquare);

    return moveStr;
}

bool Board::isInvalidMove(int &index) {
    return 0 > index || index > BOARD_LENGTH - 1;
}

void Board::debugPrint() {
    printBoard();
    std::cout << "Valid moves: ";
    printValidMoves();
    std::cout << getBoardAsFen() << "\n";
}

void Board::flipSidesToPlay() {
    sideToPlay ^= PIECE_COLOUR_MASK;
}

Board::~Board() {
    delete lionMoveGenerator;
    delete zebraMoveGenerator;
    delete giraffeMoveGenerator;
    delete pawnMoveGenerator;
    delete superPawnMoveGenerator;
}
