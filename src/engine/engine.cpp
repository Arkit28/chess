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

ChessEngine::ChessEngine(EngineLevel level) : level_(level), maxDepth_(3), timeLimit_(5000),
    nodesSearched_(0), lastEvaluation_(0.0f), lastDepth_(0){

    switch(level_){
        case EngineLevel::RANDOM:     maxDepth_ = 0; break;
        case EngineLevel::BEGINNER:   maxDepth_ = 2; break;
        case EngineLevel::EASY:       maxDepth_ = 3; break;
        case EngineLevel::MEDIUM:     maxDepth_ = 4; break;
        case EngineLevel::HARD:       maxDepth_ = 5; break;
        case EngineLevel::EXPERT:     maxDepth_ = 6; break;
    }
}

Move ChessEngine::getBestMove(const Board& board, int timelimit){
    return getBestMove(board, maxDepth_, timeLimit_);
}

Move ChessEngine::getBestMove(const Board& board, int depth, int timeLimit){
    startSearch();
    timeLimit_ = timeLimit;
    nodesSearched_ = 0;
    lastDepth_ = depth;

    std::vector<Move> legalMoves = const_cast<Board&>(board).generateLegalMoves();

    if(legalMoves.empty()){
        return {-1, -1, EMPTY, EMPTY, 0}; //no legal moves(stalemate/checkmate)
    }

    if(level_ == EngineLevel::RANDOM){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, legalMoves.size() -1);
        return legalMoves[dis(gen)];
    }

    Move bestMove = legalMoves[0];
    float bestScore = -std::numeric_limits<float>::infinity();

    std::vector<Move> orderedMoves = orderMoves(board, legalMoves);

    for(Move& move : orderedMoves){
        if(isTimeUp()) break;

        Board testBoard = board;
        testBoard.makeMove(move);
        testBoard.whiteToMove = !testBoard.whiteToMove;

        float score = -alphaBeta(testBoard, depth -1, std::numeric_limits<float>::infinity(),
                                std::numeric_limits<float>::infinity(), false);
        
        if(score > bestScore){
            bestScore = score;
            bestMove = move;
        }
    }

    lastEvaluation_ = bestScore;
    return bestMove;

}