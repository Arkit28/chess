#pragma once
#include <array>
#include <string>

enum Piece{
    EMPTY = 0,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
};

class Board {
public:
    std::array<int, 64> squares{};

    Board();

    void setStartPos();
    void print() const;
    std::string toFEN() const;
};