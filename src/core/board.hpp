#pragma once
#include "move.hpp"
#include <array>
#include <string>

enum Piece{
    EMPTY = 0,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
};

struct CastlingRights {
    bool W_KingSide;
    bool W_QueenSide;
    bool B_KingSide;
    bool B_QueenSide;
};

class Board {
public:
    std::array<int, 64> squares{};

    Board();

    void makeMove(Move&);

    CastlingRights castlingrights;
    void UpdateCastlingRights(Move& m);

    void setStartPos();
    void print() const;
    std::string toFEN() const;
};