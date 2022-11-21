#pragma once

// Used to get the colour of a piece - 11000
const static int PIECE_COLOUR_MASK = 24;

// Used to get the type of piece - 00111
const static int PIECE_TYPE_MASK = 7;

enum Piece {
    None = 0,
    Lion = 1,
    Pawn = 2,
    SuperPawn = 3,
    Zebra = 4,
    Giraffe = 5,
    White = 8,
    Black = 16
};
