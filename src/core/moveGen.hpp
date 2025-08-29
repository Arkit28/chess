#pragma once
#include "Board.hpp"
#include "move.hpp"
#include <vector>

class MoveGen {
public:
    static std::vector<Move> GenPseudoLegal(const Board& b, bool whiteToMove);


    static void addPawnMoves(const Board& b, int square, bool white, std::vector<Move>& moves);
    static void addKnightMoves(const Board& b, int square, bool white, std::vector<Move>& moves);

};