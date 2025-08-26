#include <iostream>

enum SquareCode{
    EMPTY = 0,
    B_PAWN,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING,
    W_PAWN,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING,

};

int board[64];

std::string EnumToChar(int square){
    switch (square){
        case EMPTY:
            return ".";
        case B_PAWN:
            return "p";
        case B_BISHOP:
            return "b";
        case B_KNIGHT:
            return "h";
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
            return "H";
        case W_ROOK:
            return "R";
        case W_QUEEN:
            return "Q";
        case W_KING:
            return "K";
    }
    return "";
}

void printBoard(const int board[64]){
    for(int i = 0; i < 64 ; i++){
        if(i % 8 == 0){
            std::cout << std::endl;
        }
        std::cout << EnumToChar(board[i]) << " | ";
    }

}

std::string boardToFEN(const int board[64]){
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
        if(board[i] == EMPTY){
            emptyCount++;
        }
        else{
            if(emptyCount > 0){
                fen += std::to_string(emptyCount);
                emptyCount = 0;
                
            }
            fen += EnumToChar(board[i]);
        }
        //if(emptyCount > 0){
          //  fen += std::to_string(board[i]);
        //}
    }
    return fen;
}


int main() {
    board[0] = W_ROOK;
    board[1] = W_KNIGHT;
    board[2] = W_BISHOP;
    board[3] = W_QUEEN;
    board[4] = W_KING;
    board[5] = W_BISHOP;
    board[6] = W_KNIGHT;
    board[7] = W_ROOK;
    board[8] = W_PAWN;
    board[9] = W_PAWN;
    board[10] = W_PAWN;
    board[11] = W_PAWN;
    board[12] = W_PAWN;
    board[13] = W_PAWN;
    board[14] = W_PAWN;
    board[15] = W_PAWN;

    board[48] = B_PAWN;
    board[49] = B_PAWN;
    board[50] = B_PAWN;
    board[51] = B_PAWN;
    board[52] = B_PAWN;
    board[53] = B_PAWN;
    board[54] = B_PAWN;
    board[55] = B_PAWN;
    board[56] = B_ROOK;
    board[57] = B_KNIGHT;
    board[58] = B_BISHOP;
    board[59] = B_QUEEN;
    board[60] = B_KING;
    board[61] = B_BISHOP;
    board[62] = B_KNIGHT;
    board[63] = B_ROOK;

    
    printBoard(board);
    std::cout << std::endl;
    std::cout << boardToFEN(board) << std::endl;

}