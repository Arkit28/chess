#pragma once
#include "../core/board.hpp"  // Include for Piece enum

// piece_tables.hpp - PeSTO's Evaluation Function Tables
// Separate file for clean organization and easy tuning

namespace PieceSquareTables {
    
    // Game phase calculation weights
    extern const int GAME_PHASE_INC[13];
    
    // Material values (separate for MG/EG)
    extern const int MG_PIECE_VALUES[13];
    extern const int EG_PIECE_VALUES[13];
    
    // Middlegame Piece-Square Tables (from white's perspective)
    extern const int MG_PAWN_TABLE[64];
    extern const int MG_KNIGHT_TABLE[64];
    extern const int MG_BISHOP_TABLE[64];
    extern const int MG_ROOK_TABLE[64];
    extern const int MG_QUEEN_TABLE[64];
    extern const int MG_KING_TABLE[64];
    
    // Endgame Piece-Square Tables
    extern const int EG_PAWN_TABLE[64];
    extern const int EG_KNIGHT_TABLE[64];
    extern const int EG_BISHOP_TABLE[64];
    extern const int EG_ROOK_TABLE[64];
    extern const int EG_QUEEN_TABLE[64];
    extern const int EG_KING_TABLE[64];
    
    // Pointer arrays for easy access
    extern const int* MG_PIECE_TABLES[6];
    extern const int* EG_PIECE_TABLES[6];
    
    // Utility functions (inline - can use Piece enum now)
    inline int getPieceType(int piece) {
        if (piece == EMPTY) return -1;
        return (piece - 1) / 2;     // Converts piece enum to 0-5 range
    }
    
    inline int flipSquare(int sq) {
        return sq ^ 56;  // Flips rank: rank 0 <-> rank 7, etc.
    }
    
    // Main evaluation functions
    int calculateGamePhase(const Board& board);
    float evaluateTapered(const Board& board);
}