#include "engine.hpp"
#include "../core/moveGen.hpp"
#include "../core/utils.hpp"
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

void ChessEngine::setLevel(EngineLevel level) {
    level_ = level;
    switch(level_) {
        case EngineLevel::RANDOM:     maxDepth_ = 0; break;
        case EngineLevel::BEGINNER:   maxDepth_ = 2; break;
        case EngineLevel::EASY:       maxDepth_ = 3; break;
        case EngineLevel::MEDIUM:     maxDepth_ = 4; break;
        case EngineLevel::HARD:       maxDepth_ = 5; break;
        case EngineLevel::EXPERT:     maxDepth_ = 6; break;
    }
}

const int ChessEngine::PAWN_TABLE[64] = {0};
const int ChessEngine::KNIGHT_TABLE[64] = {0};
const int ChessEngine::BISHOP_TABLE[64] = {0};  
const int ChessEngine::ROOK_TABLE[64] = {0};
const int ChessEngine::QUEEN_TABLE[64] = {0};
const int ChessEngine::KING_MG_TABLE[64] = {0};
const int ChessEngine::KING_EG_TABLE[64] = {0};


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

float ChessEngine::alphaBeta(Board& board, int depth, float alpha, float beta, bool maximisingPlayer){
    nodesSearched_++;

    if(nodesSearched_ % 1000 == 0 && isTimeUp()){
        return 0; 
    }

    if(depth == 0){
        return evaluatePosition(board);
    }

    std::vector<Move> legalMoves = board.generateLegalMoves();

    if(legalMoves.empty()){
        if(board.isCheck(board.whiteToMove)){
            return maximisingPlayer ? -20000 + (maxDepth_ - depth) : 20000 - (maxDepth_ - depth);
        }
        else{
            return 0;
        }
    }

    std::vector<Move> orderedMoves = orderMoves(board, legalMoves);

    if(maximisingPlayer){
        float maxEval = -std::numeric_limits<float>::infinity();
        for(Move& move : orderedMoves){
            Board testBoard = board;
            testBoard.whiteToMove = !testBoard.whiteToMove;

            float eval = alphaBeta(testBoard, depth -1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if((beta <= alpha)) break;
        }
        return maxEval;
    }
    else{
        float minEval = std::numeric_limits<float>::infinity();
        for(Move& move : orderedMoves){
            Board testBoard = board;
            testBoard.makeMove(move);
            testBoard.whiteToMove = !testBoard.whiteToMove;

            float eval = alphaBeta(testBoard, depth-1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if(beta <= alpha) break;
        }
        return minEval;
    }
}

float ChessEngine::evaluatePosition(const Board& board){
    float score = 0.0f;

    score += evaluateMaterial(board);

    if(level_ >= EngineLevel::EASY){
        score += evaluatePieceSquares(board) * 0.3f;
        score += evaluateMobility(board) * 0.1f;
    }

    if(level_ >= EngineLevel::MEDIUM){
        score += evaluateKingSafety(board) * 0.2f;
        score += evaluatePawnStructure(board) * 0.1f;
    }

    return board.whiteToMove ? score : -score;
}

float ChessEngine::evaluateMaterial(const Board& board){
    float score = 0;

    for(int sq = 0; sq < 64; ++sq){
        int piece = board.squares[sq];
        if(piece == EMPTY) continue;

        if(piece >= W_PAWN){
            score += PIECE_VALUES[piece];
        }
        else{
            score -= PIECE_VALUES[piece];
        }
    }

    return score;
}

float ChessEngine::evaluateMobility(const Board& board){
    Board tempBoard = board;

    tempBoard.whiteToMove = true;
    int whiteMoves = tempBoard.generateLegalMoves().size();

    tempBoard.whiteToMove = false;
    int blackMoves = tempBoard.generateLegalMoves().size();

    return (whiteMoves - blackMoves) * 0.3f;
}

std::vector<Move> ChessEngine::orderMoves(const Board& board, const std::vector<Move>& moves){
    std::vector<std::pair<Move, int>> scoredMoves;

    for(const Move& move : moves) {
        int score = getMoveOrderScore(board, move);
        scoredMoves.push_back({move, score});
    }

    //sort by highest score
    std::sort(scoredMoves.begin(), scoredMoves.end(),[](const auto&a, const auto& b) {return a.second > b.second; });

    std::vector<Move> orderedMoves;
    for(const auto& pair : scoredMoves){
        orderedMoves.push_back(pair.first);
    }

    return orderedMoves;
}

int ChessEngine::getMoveOrderScore(const Board& board, const Move& move){
    int score = 0;

    if(move.flags & CAPTURE){
        score += 1000;
        //difference in value of pieces in capture move (capturee - capturer)
        score += PIECE_VALUES[move.captured] - PIECE_VALUES[board.squares[move.from]] / 10;
    }

    if(move.flags & PROMOTION){
        score += 900;
    }

    if(move.flags & CASTLING){
        score += 100;
    }

    int toFile = file(move.to);
    int toRank = rank(move.to);
    if(toFile >= 2 && toFile <= 5 && toRank >= 2 && toRank <= 5){
        score += 10;
    }

    return score;
}

void ChessEngine::startSearch(){
    searchStartTime_ = std::chrono::steady_clock::now();
}

bool ChessEngine::isTimeUp() const{
    auto elapsed = std::chrono::steady_clock::now() - searchStartTime_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() >= timeLimit_;
}

float ChessEngine::evaluatePieceSquares(const Board& board){
    //TO DO

    return 0.0f;
}

float ChessEngine::evaluateKingSafety(const Board& board){
    //TO DO
    return 0.0f;
}

float ChessEngine::evaluatePawnStructure(const Board& board){
    //To Do
    return 0.0f;
}

float ChessEngine::minimax(Board& board, int depth, bool maximizingPlayer) {
    // TODO: Implement minimax (can use alphaBeta for now)
    return alphaBeta(board, depth, -std::numeric_limits<float>::infinity(), 
                     std::numeric_limits<float>::infinity(), maximizingPlayer);
}

float ChessEngine::quiescenceSearch(Board& board, float alpha, float beta, bool maximizingPlayer) {
    // TODO: Implement quiescence search
    return evaluatePosition(board);
}

uint64_t ChessEngine::hashPosition(const Board& board) {
    // TODO: Implement position hashing
    return 0;
}

void ChessEngine::storeTTEntry(uint64_t key, float score, int depth, int flag, const Move& bestMove) {
    // TODO: Implement transposition table storage
}

bool ChessEngine::probeTTEntry(uint64_t key, int depth, float alpha, float beta, float& score, Move& bestMove) {
    // TODO: Implement transposition table lookup
    return false;
}