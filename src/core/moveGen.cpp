#include "moveGen.hpp"
#include "utils.hpp"

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


void MoveGen::addPawnMoves(const Board& b, int square, bool white, std::vector<Move>& moves){
    
    int W_forward = square + 8;
    int W_forward2 = square + 16;
    int W_capture_left = square + 7;
    int W_capture_right = square + 9;

    int B_forward = square - 8;
    int B_forward2 = square - 16;
    int B_capture_left = square - 9;
    int B_capture_right = square - 7;

    if(white){
        // White pawn forward moves
        if(onBoard(W_forward) && b.squares[W_forward] == EMPTY){
            moves.push_back({square, W_forward, EMPTY, EMPTY, MoveFlags::QUIET});
            // Double pawn push from starting rank (rank 1)
            if(onBoard(W_forward2) && b.squares[W_forward2] == EMPTY && rank(square) == 1){
                moves.push_back({square, W_forward2, EMPTY, EMPTY, MoveFlags::DOUBLE_PAWN_PUSH});
            }
        }
        // White pawn captures
        if(file(square) != 0 && onBoard(W_capture_left) && isBlack(b.squares[W_capture_left])){
            moves.push_back({square, W_capture_left, b.squares[W_capture_left], EMPTY, MoveFlags::CAPTURE});
        }
        if(file(square) != 7 && onBoard(W_capture_right) && isBlack(b.squares[W_capture_right])){
            moves.push_back({square, W_capture_right, b.squares[W_capture_right], EMPTY, MoveFlags::CAPTURE});
        }
    }
    else{
        // Black pawn forward moves
        if(onBoard(B_forward) && b.squares[B_forward] == EMPTY){
            moves.push_back({square, B_forward, EMPTY, EMPTY, MoveFlags::QUIET});
            // Double pawn push from starting rank (rank 6)
            if(onBoard(B_forward2) && b.squares[B_forward2] == EMPTY && rank(square) == 6){
                moves.push_back({square, B_forward2, EMPTY, EMPTY, MoveFlags::DOUBLE_PAWN_PUSH});
            }
        }
        // Black pawn captures - FIXED: swapped file checks
        if(file(square) != 0 && onBoard(B_capture_left) && isWhite(b.squares[B_capture_left])){
            moves.push_back({square, B_capture_left, b.squares[B_capture_left], EMPTY, MoveFlags::CAPTURE});
        }
        if(file(square) != 7 && onBoard(B_capture_right) && isWhite(b.squares[B_capture_right])){
            moves.push_back({square, B_capture_right, b.squares[B_capture_right], EMPTY, MoveFlags::CAPTURE});
        }
    }
}


void MoveGen::addKnightMoves(const Board& b, int square, bool white, std::vector<Move>& moves) {
    // TODO: implement knight moves
}
