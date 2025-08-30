#include "moveGen.hpp"
#include "utils.hpp"
#include <iostream>

bool isWhite(const int piece){
    return piece == W_BISHOP || piece == W_KING || piece == W_KNIGHT || piece == W_ROOK || piece == W_QUEEN || piece == W_PAWN;
}

bool isBlack(const int piece){
    return piece == B_BISHOP || piece == B_KING || piece == B_KNIGHT || piece == B_ROOK || piece == B_QUEEN || piece == B_PAWN;

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
        }
        else if(!whiteToMove && isBlack(piece)){
            if(piece == B_PAWN){
                addPawnMoves(b, sq, false, moves);
            }
            else if(piece == B_KNIGHT){
                addKnightMoves(b, sq, false, moves);
            }
        }
    }
    return moves;
}

void debugCoordinateSystem() {
    std::cout << "Coordinate system analysis:\n";
    std::cout << "Square -> File, Rank, Chess Notation\n";
    
    // Check some key squares
    for(int sq = 0; sq < 64; sq += 8) {
        std::cout << "Square " << sq << " -> file " << file(sq) << ", rank " << rank(sq) 
                  << " (" << fileChar(sq) << rankChar(sq) << ")\n";
    }
    
    // Check your specific test squares
    std::cout << "\nYour test position squares:\n";
    std::cout << "e2 should be: file 4, rank 1 = " << (1*8 + 4) << "\n";
    std::cout << "d5 should be: file 3, rank 4 = " << (4*8 + 3) << "\n"; 
    std::cout << "f5 should be: file 5, rank 4 = " << (4*8 + 5) << "\n";
    std::cout << "e4 should be: file 4, rank 3 = " << (3*8 + 4) << "\n";
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
        if(file(square) != 0 && onBoard(capture_left) && isBlack(b.squares[capture_left])){            moves.push_back({square, capture_left, b.squares[capture_left], EMPTY, MoveFlags::CAPTURE});
        }
        if(file(square) != 7 && onBoard(capture_right) && isBlack(b.squares[capture_right])){            moves.push_back({square, capture_right, b.squares[capture_right], EMPTY, MoveFlags::CAPTURE});
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
            std::cout << "target square index: " << target << std::endl;
            if(onBoard(target) && !isWhite(b.squares[target])){
                if(isBlack(b.squares[target])){
                    moves.push_back({square, target, CAPTURE, EMPTY, MoveFlags::CAPTURE});
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
                    moves.push_back({square, target, CAPTURE, EMPTY, MoveFlags::CAPTURE});
                }
                else{
                    moves.push_back({square, target, EMPTY, EMPTY, MoveFlags::QUIET});
                }
            }
        }

    }

}
