#pragma once
#include "move.hpp"
#include <array>
#include <string>
#include <vector>

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

    void updateGameState(const Move& move);
    int findking(bool white) const;
    bool isCheck(bool white) const;
    bool isSquareAttacked(int square, bool byWhite) const;

    std::vector<Move> generateLegalMoves();

    void makeMove(Move&);
    bool whiteToMove = true;
    int enPassantSquare = -1;
    int halfmoveClock = 0;
    int fullMoveNumber = 1;

    CastlingRights castlingrights;
    void UpdateCastlingRights(Move& m);

    void setStartPos();
    void print() const;
    std::string toFEN() const;

    int parseSquare(const std::string square);
    Move parseMove(const std::string& Move);
    Move findMatchingMove(const std::vector<Move>& legalMoves, const Move& inputMove);
    bool IsMoveLegal(const Move& move, const std::vector<Move>& legalMoves);

    bool isCheckmate();
    bool isStalemate();
};