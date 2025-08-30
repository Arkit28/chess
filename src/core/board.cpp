#include "board.hpp"
#include "utils.hpp"
#include <iostream>

std::string EnumToChar(int square){
    switch (square){
        case EMPTY:
            return ".";
        case B_PAWN:
            return "p";
        case B_BISHOP:
            return "b";
        case B_KNIGHT:
            return "n";
        case B_ROOK:
            return "r";
        case B_QUEEN:
            return "q";
        case B_KING:
            return "k";
        case W_PAWN:
            return "P";
        case W_BISHOP:
            return "B";
        case W_KNIGHT:
            return "N";
        case W_ROOK:
            return "R";
        case W_QUEEN:
            return "Q";
        case W_KING:
            return "K";
    }
    return "";
}

Board::Board() {
    squares.fill(EMPTY);
}

void Board::setStartPos() {
    squares[0] = W_ROOK;
    squares[1] = W_KNIGHT;
    squares[2] = W_BISHOP;
    squares[3] = W_QUEEN;
    squares[4] = W_KING;
    squares[5] = W_BISHOP;
    squares[6] = W_KNIGHT;
    squares[7] = W_ROOK;
    squares[8] = W_PAWN;
    squares[9] = W_PAWN;
    squares[10] = W_PAWN;
    squares[11] = W_PAWN;
    squares[12] = W_PAWN;
    squares[13] = W_PAWN;
    squares[14] = W_PAWN;
    squares[15] = W_PAWN;

    squares[48] = B_PAWN;
    squares[49] = B_PAWN;
    squares[50] = B_PAWN;
    squares[51] = B_PAWN;
    squares[52] = B_PAWN;
    squares[53] = B_PAWN;
    squares[54] = B_PAWN;
    squares[55] = B_PAWN;
    squares[56] = B_ROOK;
    squares[57] = B_KNIGHT;
    squares[58] = B_BISHOP;
    squares[59] = B_QUEEN;
    squares[60] = B_KING;
    squares[61] = B_BISHOP;
    squares[62] = B_KNIGHT;
    squares[63] = B_ROOK;
}

void Board::print() const {
    for(int r = 7; r >= 0; r--){
        std::cout << r+1 << "  ";
        for(int f = 0; f < 8; f++){
            int sq = r * 8 + f;
            std::cout << EnumToChar(squares[sq]) << " ";
       
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n"; // file letters
}


std::string Board::toFEN() const{
        std::string fen;
    int emptyCount = 0;
    for(int i = 0; i < 64; i++){

        if(i % 8 == 0 && i != 0 && i != 63){
            if(emptyCount > 0){
                fen += std::to_string(emptyCount);
                emptyCount = 0;
            }
            fen += '/';
        }
        if(squares[i] == EMPTY){
            emptyCount++;
        }
        else{
            if(emptyCount > 0){
                fen += std::to_string(emptyCount);
                emptyCount = 0;
                
            }
            fen += EnumToChar(squares[i]);
        }
        //if(emptyCount > 0){
          //  fen += std::to_string(board[i]);
        //}
    }
    return fen;
}

