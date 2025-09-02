#include "moveGen.hpp"
#include "utils.hpp"
#include <iostream>

bool isWhite(const int piece){
    return piece == W_BISHOP || piece == W_KING || piece == W_KNIGHT || piece == W_ROOK || piece == W_QUEEN || piece == W_PAWN;
}

bool isBlack(const int piece){
    return piece == B_BISHOP || piece == B_KING || piece == B_KNIGHT || piece == B_ROOK || piece == B_QUEEN || piece == B_PAWN;

}

bool isSquareAttacked(const Board& b, int square, bool byWhite){
    if(byWhite){
        if(onBoard(square - 7) && b.squares[square-7] == W_PAWN) return true;
        if(onBoard(square - 9) && b.squares[square-9] == W_PAWN) return true;
    }
    else{
        if(onBoard(square + 7) && b.squares[square+7] == B_PAWN) return true;
        if(onBoard(square + 9) && b.squares[square+9] == B_PAWN) return true;
    }

    int knightOffset[8] = {6, -6, 10, -10, 15, -15, 17, -17};
    for(int offset : knightOffset){
        int target = square + offset;
        if(onBoard(target)){
            if(b.squares[target] == W_KNIGHT && byWhite) return true;
            if(b.squares[target] == B_KNIGHT && !byWhite) return true;
        }
    }

    auto isFriendly = !byWhite ? isWhite : isBlack;
    int direction[4] = {1, -1, 8 , -8};
    for(const auto& dir : direction){
        int target = square + dir;
        while(onBoard(target)){
            if((dir == -1 || dir == 1) && rank(target) != rank (square)) break;

            if(b.squares[target] == EMPTY){
                target += dir;
                continue;
            }
            if(isFriendly(b.squares[target])) break;
            else if(b.squares[target] == W_ROOK || b.squares[target] == W_QUEEN 
                    || b.squares[target] == B_ROOK || b.squares[target] == B_QUEEN) return true;
            else break;
        }
    }

    int direction1[4] = {9, -9, 7 , -7};
    for(const auto& dir : direction1){
        int target = square + dir;
        while(onBoard(target)){
            int fileDiff = std::abs(file(target) - file(target-dir));
            if(fileDiff != 1) break;

            if(b.squares[target] == EMPTY) {
                target += dir;
                continue;
            }
            if(isFriendly(b.squares[target])){
                break;
            }
            if(b.squares[target] == W_QUEEN || b.squares[target] == W_BISHOP 
               || b.squares[target] == B_QUEEN || b.squares[target] == B_BISHOP){
                return true;
            }
            else break;
        }
    }

    int offset[8] = {1, -1, 8, -8, 7, -7, 9, -9};
    for(const auto& dir : offset){
        int target = square + dir;
        if(onBoard(target)){
            if(byWhite && b.squares[target] == W_KING) return true;
            if(!byWhite && b.squares[target] == B_KING) return true;
        }
    }

    return false;

}



std::vector<Move> MoveGen::GenPseudoLegal(const Board& b, bool whiteToMove){
    std::vector<Move> moves;
    for(int sq = 0; sq < 64; sq++){    
        int piece = b.squares[sq];
        if(piece == EMPTY){
            continue;
        }

        if(whiteToMove && isWhite(piece)){
            if(piece == W_PAWN){
                addPawnMoves(b, sq, true, moves);
            }
            else if(piece == W_KNIGHT){
                addKnightMoves(b, sq, true, moves);
            }
            else if(piece == W_ROOK){
                addRookMoves(b, sq, true, moves);
            }
            else if(piece == W_BISHOP){
                addBishopMoves(b, sq, true, moves);
            }
            else if(piece == W_QUEEN){
                addQueenMoves(b, sq, true, moves);
            }
        }
        else if(!whiteToMove && isBlack(piece)){
            if(piece == B_PAWN){
                addPawnMoves(b, sq, false, moves);
            }
            else if(piece == B_KNIGHT){
                addKnightMoves(b, sq, false, moves);
            }
            else if(piece == B_ROOK){
                addRookMoves(b, sq, false, moves);
            }
            else if(piece == B_BISHOP){
                addBishopMoves(b, sq, false, moves);
            }
            else if(piece == B_QUEEN){
                addQueenMoves(b, sq, true, moves);
            }
        }
    }
    return moves;
}


void MoveGen::addPawnMoves(const Board& b, int square, bool white, std::vector<Move>& moves){
      
    if(white){
        int forward = square + 8;
        int forward2 = square + 16;
        int capture_left = square + 7;   
        int capture_right = square + 9;  

        //White pawn forward moves
        if(onBoard(forward) && b.squares[forward] == EMPTY){
            moves.push_back({square, forward, EMPTY, EMPTY, MoveFlags::QUIET});
            if(onBoard(forward2) && b.squares[forward2] == EMPTY && rank(square) == 1){
                moves.push_back({square, forward2, EMPTY, EMPTY, MoveFlags::DOUBLE_PAWN_PUSH});
            }
        }
        
        //White pawn captures
        if(file(square) != 0 && onBoard(capture_left) && isBlack(b.squares[capture_left])){
                moves.push_back({square, capture_left, b.squares[capture_left], EMPTY, MoveFlags::CAPTURE});
        }
        if(file(square) != 7 && onBoard(capture_right) && isBlack(b.squares[capture_right])){ 
               moves.push_back({square, capture_right, b.squares[capture_right], EMPTY, MoveFlags::CAPTURE});
        }
    }
    else{
        int forward = square - 8;        
        int forward2 = square - 16;      
        int capture_left = square - 9;   
        int capture_right = square - 7;  

        //Black pawn forward moves
        if(onBoard(forward) && b.squares[forward] == EMPTY){
            moves.push_back({square, forward, EMPTY, EMPTY, MoveFlags::QUIET});
            if(onBoard(forward2) && b.squares[forward2] == EMPTY && rank(square) == 6){
                moves.push_back({square, forward2, EMPTY, EMPTY, MoveFlags::DOUBLE_PAWN_PUSH});
            }
        }
        
        //Black pawn captures
        if(file(square) != 0 && onBoard(capture_left) && isWhite(b.squares[capture_left])){
            moves.push_back({square, capture_left, b.squares[capture_left], EMPTY, MoveFlags::CAPTURE});
        }
        if(file(square) != 7 && onBoard(capture_right) && isWhite(b.squares[capture_right])){
            moves.push_back({square, capture_right, b.squares[capture_right], EMPTY, MoveFlags::CAPTURE});
        }
    }
}


void MoveGen::addKnightMoves(const Board& b, int square, bool white, std::vector<Move>& moves) {
    //moves in L shapes from current position
    //in theory need to workout four movements, then invert them
    // also need to check whether moves are captures or quiet

    //Use knight move offsets
    const int knightOffsets[8] = {17, 15, 10, 6, -17, -15, -10, -6};
    if(white){
        for(int i = 0; i < 8; i++){         // for each possible move of the knight
            int target = square + knightOffsets[i];
            if(onBoard(target) && !isWhite(b.squares[target])){
                if(isBlack(b.squares[target])){
                    moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::CAPTURE});
                }
                else{
                    moves.push_back({square, target, EMPTY, EMPTY, MoveFlags::QUIET});
                }
            }
        }
    }
    else{
        for(int i = 0; i < 8; i++){         // for each possible move of the knight
            int target = square + knightOffsets[i];
            if(onBoard(target) && !isBlack(b.squares[target])){
                if(isWhite(b.squares[target])){
                    moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::CAPTURE});
                }
                else{
                    moves.push_back({square, target, EMPTY, EMPTY, MoveFlags::QUIET});
                }
            }
        }

    }

}

void MoveGen::addRookMoves(const Board& b, int square, bool white, std::vector<Move>& moves){
    int direction[4] = {1, -1, 8, -8};

    auto isFriendly = white ? isWhite : isBlack;
    for(int dir : direction){
        int target = square + dir;
        while(onBoard(target)){
            if((dir == -1 || dir == 1) && rank(square) != rank(target)) break;

            if(isFriendly(b.squares[target])) break;

            if(b.squares[target] == EMPTY){
                moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::QUIET});
            }
            else{
                moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::CAPTURE});
                break;
            }
            target += dir;
        }
    }
}

void MoveGen::addBishopMoves(const Board& b, int square, bool white, std::vector<Move>& moves){
    int direction[4] = {7, -7, 9, -9};

    auto isFriendly = white ? isWhite : isBlack;
    for(int dir : direction){
        int target = square + dir;
        while(onBoard(target)){

            int fileDiff = std::abs(file(target) - file(target-dir));
            if(fileDiff != 1) break;

            if(isFriendly(b.squares[target])) break;

            if(b.squares[target] == EMPTY){
                moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::QUIET});
            }
            else{
                moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::CAPTURE});
                break;
            }
            target += dir;
        }
    }
}


void MoveGen::addQueenMoves(const Board& b, int square, bool white, std::vector<Move>& moves){
    addBishopMoves(b,square, white, moves);
    addRookMoves(b, square, white, moves);

}

void MoveGen::addKingMoves(const Board& b, int square, bool white, std::vector<Move>& moves){
    int direction[8] = {1, -1, 8, -8, 7, -7, 9, -9};

    // TODO
    //castling
    // conditions: king and rook havent moved since the start of the game 
    // and king hasnt been checked or is currently in check
    

    for(int dir : direction){
        int target = square + dir;
        while(onBoard(target)){
            if(isWhite(b.squares[target])){
                continue;
            }
            else if(b.squares[target] == EMPTY){
                moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::QUIET});
            }
            else{
                moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::CAPTURE});
            }
        }
    }
}


// can optimise Bishop, Rook and Queen movements by creating addSlidingMoves() function in future.

