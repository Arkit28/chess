#pragma once
#include "../core/board.hpp"
#include "../core/move.hpp"
#include <vector>
#include <chrono>
#include <unordered_map>

enum class EngineLevel {
    RANDOM = 0,
    BEGINNER,         //Depth 1-2 - Basic Eval
    EASY,             //Depth 2-3 - Improved Eval
    MEDIUM,           //Depth 3-4 - Better Search,
    HARD,             //Depth 4-5 - Advanced techniques
    EXPERT            //Depth 5+  - Fully optimised
};

// transposition table entry
struct TTEntry {
    uint64_t key;       //Position hashkey
    float score;        //Evaluation score
    int depth;          //Search depth
    int flag;           //Exact, lower bound, or upper bound
    Move bestMove;      //Best move from this position
};

class ChessEngine {
public: 
    ChessEngine(EngineLevel level = EngineLevel::EASY);    // default engine level is EASY

    //Main interface
    Move getBestMove(const Board& board, int timelimit=5000);
    Move getBestMove(const Board& board, int depth, int timelimit);

    //Engine configuration
    void setLevel(EngineLevel level);
    void setTimeLimit(int milliseconds) { timeLimit_ = milliseconds; }
    void setMaxDepth(int depth) { maxDepth_ = depth; }

    //Statistics
    int getNodesSearched() const { return nodesSearched_; }
    float getLastEvaluation() const { return lastEvaluation_; }
    int getLastDepth() const { return lastDepth_; }

private:
    //SEARCH ALGORITHMS
    float minimax(Board& board, int depth, bool maximizingPlayer);
    float alphaBeta(Board& board, int depth, float alpha, float beta, bool maximizingPlayer);
    float quiescenceSearch(Board& board, float alpha, float beta, bool maximizingPlayer);
    
    //EVALUATION FUNCTIONS
    float evaluatePosition(const Board& board);
    float evaluateMaterial(const Board& board);
    float evaluatePieceSquares(const Board& board);
    float evaluateKingSafety(const Board& board);
    float evaluatePawnStructure(const Board& board);
    float evaluateMobility(const Board& board);
    
    //MOVE ORDERING
    std::vector<Move> orderMoves(const Board& board, const std::vector<Move>& moves);
    int getMoveOrderScore(const Board& board, const Move& move);
    
    //UTILITY FUNCTIONS
    bool isTimeUp() const;
    void startSearch();
    uint64_t hashPosition(const Board& board);
    
    //TRANSPOSITION TABLE
    void storeTTEntry(uint64_t key, float score, int depth, int flag, const Move& bestMove);
    bool probeTTEntry(uint64_t key, int depth, float alpha, float beta, float& score, Move& bestMove);
    
    //Engine settings
    EngineLevel level_;
    int maxDepth_;
    int timeLimit_;
    
    //Search state
    int nodesSearched_;
    float lastEvaluation_;
    int lastDepth_;
    std::chrono::steady_clock::time_point searchStartTime_;
    
    //Transposition table
    std::unordered_map<uint64_t, TTEntry> transpositionTable_;
    
    //Piece values for evaluation
    static const int PIECE_VALUES[13];
    
    //Piece-square tables for positional evaluation
    static const int PAWN_TABLE[64];
    static const int KNIGHT_TABLE[64];
    static const int BISHOP_TABLE[64];
    static const int ROOK_TABLE[64];
    static const int QUEEN_TABLE[64];
    static const int KING_MG_TABLE[64];  // King middlegame
    static const int KING_EG_TABLE[64];  // King endgame
};

