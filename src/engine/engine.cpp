#include "engine.hpp"
#include "../core/moveGen.hpp"
#include <algorithm>
#include <random>
#include <limits>
#include <iostream>

//Piece values in centipawns (100 = 1 pawn)
const int ChessEngine::PIECE_VALUES[13] = {
    0,    
    100,  // B_PAWN
    320,  // B_KNIGHT  
    330,  // B_BISHOP
    500,  // B_ROOK
    900,  // B_QUEEN
    20000, // B_KING
    100,  // W_PAWN
    320,  // W_KNIGHT
    330,  // W_BISHOP
    500,  // W_ROOK
    900,  // W_QUEEN
    20000  // W_KING
};