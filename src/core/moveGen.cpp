#include "moveGen.hpp"
#include "utils.hpp"
#include <iostream>
#include <cmath>

bool isWhite(const int piece){
    return piece == W_BISHOP || piece == W_KING || piece == W_KNIGHT || piece == W_ROOK || piece == W_QUEEN || piece == W_PAWN;
}

bool isBlack(const int piece){
    return piece == B_BISHOP || piece == B_KING || piece == B_KNIGHT || piece == B_ROOK || piece == B_QUEEN || piece == B_PAWN;

}

bool isSquareAttacked(const Board& b, int square, bool byWhite) {
    // 1) pawn attacks
    if(byWhite){
        int from = square - 7;
        if(onBoard(from) && file(from) == file(square) - 1 && b.squares[from] == W_PAWN) return true;
        from = square - 9;
        if(onBoard(from) && file(from) == file(square) + 1 && b.squares[from] == W_PAWN) return true;
    } 
    else{
        int from = square + 7;
        if(onBoard(from) && file(from) == file(square) + 1 && b.squares[from] == B_PAWN) return true;
        from = square + 9;
        if(onBoard(from) && file(from) == file(square) - 1 && b.squares[from] == B_PAWN) return true;
    }

    // 2) knights
    const int knightOffsets[8] = {6, -6, 10, -10, 15, -15, 17, -17};
    for(int d : knightOffsets){
        int t = square + d;
        if(!onBoard(t)) continue;
        int fd = std::abs(file(t) - file(square));
        int rd = std::abs(rank(t) - rank(square));
        if(!((fd == 1 && rd == 2) || (fd == 2 && rd == 1))) continue;
        if(byWhite ? b.squares[t] == W_KNIGHT : b.squares[t] == B_KNIGHT) return true;
    }

    // check attacker colour
    auto isAttackerPiece = [&](int p){
        return byWhite ? isWhite(p) : isBlack(p);
    };

    // 3) straight rays (rook/queen)
    const int rookDirs[4] = {1, -1, 8, -8};
    for(int dir : rookDirs){
        int prev = square;
        int t = square + dir;
        while(onBoard(t)){
            // prevent horizontal wrap
            if((dir == 1 || dir == -1) && rank(t) != rank(prev)) break;

            int p = b.squares[t];
            if(p == EMPTY) {
                prev = t;
                t += dir;
                continue;
            }
            // if piece belongs to attacker
            if(isAttackerPiece(p)){
                if ((byWhite && (p == W_ROOK || p == W_QUEEN)) ||
                    (!byWhite && (p == B_ROOK || p == B_QUEEN)) ) {
                    return true;
                }
                break; // attacker piece but not rook/queen -> blocked
            } 
            else{
                break; // blocked by defender piece
            }
        }
    }

    // 4) diagonals (bishop/queen)
    const int diagDirs[4] = {9, -9, 7, -7};
    for(int dir : diagDirs){
        int prev = square;
        int t = square + dir;
        while(onBoard(t)) {
            // ensure step is diagonal (file & rank step of 1)
            if (std::abs(file(t) - file(prev)) != 1 || std::abs(rank(t) - rank(prev)) != 1) break;

            int p = b.squares[t];
            if(p == EMPTY){
                prev = t;
                t += dir;
                continue;
            }
            if(isAttackerPiece(p)) {
                if((byWhite && (p == W_BISHOP || p == W_QUEEN)) ||
                    (!byWhite && (p == B_BISHOP || p == B_QUEEN))) {
                    return true;
                }
                break;
            } 
            else{
                break;
            }
        }
    }

    // 5) adjacent king (must check adjacency safely — avoid wrap)
    for(int dir : {1, -1, 8, -8, 7, -7, 9, -9}){
        int t = square + dir;
        if(!onBoard(t)) continue;
        int fd = std::abs(file(t) - file(square));
        int rd = std::abs(rank(t) - rank(square));
        if(fd <= 1 && rd <= 1){
            if(byWhite ? b.squares[t] == W_KING : b.squares[t] == B_KING) return true;
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
            else if(piece == W_KING){
                addKingMoves(b, sq, true, moves);
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
                addQueenMoves(b, sq, false, moves);
            }
            else if(piece == B_KING){
                addKingMoves(b, sq, false, moves);
            }
        }
    }
    return moves;
}


void MoveGen::addPawnMoves(const Board& b, int square, bool white, std::vector<Move>& moves){

    //en passant conditons: 
    // white - rank=5 and black pawn's last move must have been double_pawn_push
    // black - rank=4 and white pawn's last move must have been double_pawn_push
    // the opposing pawn must land directly adjacent to the current side's pawn
    // the pawn must be taken on that move otherwise it is gone 
      
    if(white){
        int forward = square + 8;
        int forward2 = square + 16;
        int capture_left = square + 7;   
        int capture_right = square + 9;  

        //White pawn forward moves
        if(onBoard(forward) && b.squares[forward] == EMPTY){

            if(rank(forward) == 7){
                moves.push_back({square, forward, b.squares[forward], W_QUEEN, MoveFlags::PROMOTION});  // EXPAND TO OTHER PIECES
            }
            else{
                moves.push_back({square, forward, EMPTY, EMPTY, MoveFlags::QUIET});
            }

            if(onBoard(forward2) && b.squares[forward2] == EMPTY && rank(square) == 1){
                moves.push_back({square, forward2, EMPTY, EMPTY, MoveFlags::DOUBLE_PAWN_PUSH});
            }
        }
        
        //White pawn captures
        if(file(square) != 0 && onBoard(capture_left) && isBlack(b.squares[capture_left])){
            if(rank(capture_left) == 7){
                moves.push_back({square, capture_left, b.squares[capture_left], W_QUEEN, MoveFlags::CAPTURE});

            }
            else{
                moves.push_back({square, capture_left, b.squares[capture_left], EMPTY, MoveFlags::CAPTURE});
            }
            if(file(square) != 7 && onBoard(capture_right) && isBlack(b.squares[capture_right])){ 
                if(rank(capture_right) == 7){
                moves.push_back({square, capture_right, b.squares[capture_right], W_QUEEN, MoveFlags::CAPTURE});
                }
                else{
                moves.push_back({square, capture_right, b.squares[capture_right], EMPTY, MoveFlags::CAPTURE});
                }
            }

            //white en passant - only rank 5
            if(rank(square) == 4 && b.enPassantSquare != -1){
                if(file(square) != 0 && capture_left == b.enPassantSquare){
                    moves.push_back({square, capture_left, B_PAWN, EMPTY, MoveFlags::EN_PASSANT});
                }

                if(file(square) != 7 && capture_left == b.enPassantSquare){
                    moves.push_back({square, capture_right, B_PAWN, EMPTY, MoveFlags::EN_PASSANT});
                }
            }
        }
    }
    else{
        int forward = square - 8;        
        int forward2 = square - 16;      
        int capture_left = square - 9;   
        int capture_right = square - 7;  

        //Black pawn forward moves
        if(onBoard(forward) && b.squares[forward] == EMPTY){
            if(rank(forward) == 0){
                moves.push_back({square, forward, EMPTY, B_QUEEN, MoveFlags::PROMOTION});
            }
            else{
                moves.push_back({square, forward, EMPTY, EMPTY, MoveFlags::QUIET});
            }
            if(onBoard(forward2) && b.squares[forward2] == EMPTY && rank(square) == 6){
                moves.push_back({square, forward2, EMPTY, EMPTY, MoveFlags::DOUBLE_PAWN_PUSH});
            }
        }
        
        //Black pawn captures
        if(file(square) != 0 && onBoard(capture_left) && isWhite(b.squares[capture_left])){
            if(rank(forward) == 0){
                moves.push_back({square, capture_left, b.squares[capture_left], B_QUEEN, MoveFlags::PROMOTION});
            }
            else{
                moves.push_back({square, capture_left, b.squares[capture_left], EMPTY, MoveFlags::CAPTURE});
            }
        }
        if(file(square) != 7 && onBoard(capture_right) && isWhite(b.squares[capture_right])){
            if(rank(capture_left) == 0){
                moves.push_back({square, capture_right, b.squares[capture_right], EMPTY, MoveFlags::PROMOTION});
            }
            else{
                moves.push_back({square, capture_left, b.squares[capture_left], EMPTY, MoveFlags::CAPTURE});
            }
        }
        
        //black en passant
        if(rank(square) == 3 && b.enPassantSquare != -1){
            if(file(square) != 0 && capture_left == b.enPassantSquare){
                moves.push_back({square, capture_left, W_PAWN, EMPTY, MoveFlags::PROMOTION});
            }

            if(file(square) != 7 && capture_right == b.enPassantSquare){
                moves.push_back({square, capture_right, W_PAWN, EMPTY, MoveFlags::EN_PASSANT});
            }
        }
    }
}

void MoveGen::addKnightMoves(const Board& b, int square, bool white, std::vector<Move>& moves) {
    //moves in L shapes from current position
    // also need to check whether moves are captures or quiet

    //Use knight move offsets
    const int knightOffsets[8] = {17, 15, 10, 6, -17, -15, -10, -6};
    for(int i = 0; i < 8; i++){         // for each possible move of the knight
        int target = square + knightOffsets[i];

        if(!onBoard(target)) continue; 

        int fromFile = file(square);
        int toFile = file(target);
        int fromRank = rank(square);
        int toRank = rank(target);

        int fileDiff = std::abs(toFile - fromFile);
        int rankDiff = std::abs(toRank - fromRank);

        if(!((fileDiff == 2 && rankDiff == 1) || (fileDiff == 1 && rankDiff == 2))) continue;

        if(white){
            if(!isWhite(b.squares[target])){
                if(isBlack(b.squares[target])){
                    moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::CAPTURE});
                }
                else{
                    moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::QUIET});
                }
            }
        }
        else{
            if(!isBlack(b.squares[target])){
                if(isWhite(b.squares[target])){
                    moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::CAPTURE});
                }
                else{
                    moves.push_back({square, target, b.squares[target], EMPTY, MoveFlags::QUIET});
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
    // castling
    // conditions: king and rook havent moved since the start of the game 
    // and king hasnt been checked or is currently in check
    auto isFriendly = white ? isWhite : isBlack;
    for(int dir : direction){
        int target = square + dir;
        if(onBoard(target)){

            if((dir == 7 || dir == -7 || dir == 9 || dir == -9)){
                int fileDiff = abs(file(target) - file(square));
                if(fileDiff != 1){
                    continue; 
                }
            }
            if((dir == 1 || dir == -1) && rank(target) != rank(square)) {
                continue;
            }
            if(isFriendly(b.squares[target])){
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
    
    // add castling if conditions are met
    if(white){
        if(b.castlingrights.W_QueenSide == true && b.squares[1] == EMPTY && b.squares[2] == EMPTY && b.squares[3] == EMPTY
           && !isSquareAttacked(b, 4, false)
           && !isSquareAttacked(b, 3, false)
           && !isSquareAttacked(b, 2, false)){
            moves.push_back({square, 2, b.squares[square], EMPTY, MoveFlags::CASTLING});
        }
        if(b.castlingrights.W_KingSide == true && b.squares[5] == EMPTY && b.squares[6] == EMPTY
           && !isSquareAttacked(b, 4, false)
           && !isSquareAttacked(b, 5, false)
           && !isSquareAttacked(b, 6, false)){
            moves.push_back({square, 6, b.squares[square], EMPTY, MoveFlags::CASTLING});
        }
    }
    else{
        if(b.castlingrights.B_QueenSide == true && b.squares[57] == EMPTY && b.squares[58] == EMPTY && b.squares[59] == EMPTY
           && !isSquareAttacked(b, 60, true)
           && !isSquareAttacked(b, 59, true)
           && !isSquareAttacked(b, 58, true)){
            moves.push_back({square, 57, b.squares[square], EMPTY, MoveFlags::CASTLING});
        }
        if(b.castlingrights.B_KingSide == true && b.squares[61] == EMPTY && b.squares[62] == EMPTY
           && !isSquareAttacked(b, 60, true)
           && !isSquareAttacked(b, 61, true)
           && !isSquareAttacked(b, 62, true)){
            moves.push_back({square, 62, b.squares[square], EMPTY, MoveFlags::CASTLING});
        }
    }
}


// can optimise Bishop, Rook and Queen movements by creating addSlidingMoves() function in future.

