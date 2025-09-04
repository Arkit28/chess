#include "board.hpp"
#include "utils.hpp"
#include "move.hpp"
#include "moveGen.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>

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

    castlingrights = {true, true, true, true};

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


void Board::updateGameState(const Move& move){
    UpdateCastlingRights(const_cast<Move&>(move));

    enPassantSquare = -1;
    if(move.flags == DOUBLE_PAWN_PUSH){
        if(squares[move.to] == W_PAWN){
            enPassantSquare = move.to - 8;
        }
        else{
            enPassantSquare = move.to + 8;
        }
    }

    if(move.flags == CAPTURE || squares[move.to] == W_PAWN || squares[move.to] == B_PAWN){
        halfmoveClock = 0;
    }
    else{
        halfmoveClock++;
    }

    if(!whiteToMove) fullMoveNumber++;

    whiteToMove = !whiteToMove;

}

int Board::findking(bool white) const{
    int targetKing = white ? W_KING : B_KING;

    for(int i = 0; i < 64; i++){
        if(squares[i] == targetKing){
            return i;
        }
    }
    return -1;
}

bool Board::isCheck(bool white) const{
    int kingSquare = findking(white);
    if(kingSquare == -1) return false;

    return isSquareAttacked(kingSquare, !white);
}

std::vector<Move> Board::generateLegalMoves(){
    std::vector<Move> pseudoLegalMoves = MoveGen::GenPseudoLegal(*this, whiteToMove);
    std::vector<Move> legalMoves;

    for(auto& move : pseudoLegalMoves){
        Board testBoard = *this;
        testBoard.makeMove(move);

        if(!testBoard.isCheck(whiteToMove)){
            legalMoves.push_back(move);
        }
    }
    return legalMoves;

}

bool Board::isCheckmate(){
    if(!isCheck(whiteToMove)) return false;
    return generateLegalMoves().empty();
}
   

bool Board::isStalemate(){
    if(isCheck(whiteToMove)) return false;
    return generateLegalMoves().empty();
}



std::string Board::toFEN() const{
    std::string fen;
    //board representation
    for(int rank = 7; rank >= 0; rank--){
        int emptyCount = 0;
        for(int file = 0; file < 8; file++){
            int square = rank * 8 + file;

            if(squares[square] == EMPTY){
                 emptyCount++;
            }
            else{
                if(emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                    }
                fen += EnumToChar(squares[square]);
            }
        }
        if(emptyCount > 0){
            fen += std::to_string(emptyCount);
        }
        if(rank > 0) {
            fen += '/';
        }
    }
    // active side
    fen += " ";
    fen += whiteToMove ? "w" : "b";

    // castling rights;
    fen += " ";
    std::string castling = "";

    if(castlingrights.W_KingSide) castling += "K";
    if(castlingrights.W_QueenSide) castling += "Q";
    if(castlingrights.B_KingSide) castling += "k";
    if(castlingrights.B_QueenSide) castling += "q";
    if(castling.empty()) castling = "-";
    fen += castling;

    //en passant target square
    fen += " ";
    if(enPassantSquare == -1){
        fen += "-";
    }
    else{
        fen += fileChar(enPassantSquare);
        fen += rankChar(enPassantSquare);
    }

    //halfmove clock
    fen += " ";
    fen += std::to_string(halfmoveClock);

    // fullmove clock
    fen += " ";
    fen += std::to_string(fullMoveNumber);

    return fen;
}


// need to make CAPTURE + PROMOTION simultaneous  logic
void Board::makeMove(Move& m){
    // normal moves - Flag = QUIET
    if(m.flags == QUIET || m.flags == DOUBLE_PAWN_PUSH){
        squares[m.to] = squares[m.from];
        squares[m.from] = EMPTY;
    }
    else if(m.flags == CAPTURE){
        squares[m.to] = squares[m.from];
        squares[m.from] = EMPTY;
    }
    else if(m.flags == PROMOTION){
        if(squares[m.from] == W_PAWN){
            squares[m.to] = W_QUEEN;          //to be changed
            squares[m.from] = EMPTY;
        }
        else{
            squares[m.to] = B_QUEEN;
        }
        squares[m.from] = EMPTY;
    }
    if(m.flags == CASTLING){
        if(m.to == 2){
            squares[m.to] = W_KING;
            squares[3] = W_ROOK;
            squares[m.from] = EMPTY;
            squares[0] = EMPTY;
            castlingrights.W_KingSide = castlingrights.W_QueenSide = false;
        }
        else if(m.to == 6){
            squares[m.to] = W_KING;
            squares[5] = W_ROOK;
            squares[m.from] = EMPTY;
            squares[7] = EMPTY;  
            castlingrights.W_KingSide = castlingrights.W_QueenSide = false;

        }
        else if(m.to == 58){
            squares[m.to] = W_KING;
            squares[59] = W_ROOK;
            squares[m.from] = EMPTY;
            squares[56] = EMPTY;
            castlingrights.B_KingSide = castlingrights.B_QueenSide = false;
        }
        else if(m.to == 62){
            squares[m.to] = W_KING;
            squares[61] = W_ROOK;
            squares[m.from] = EMPTY;
            squares[63] = EMPTY;
            castlingrights.B_KingSide = castlingrights.B_QueenSide = false;

        }
    }
}



void Board::UpdateCastlingRights(Move& m){
    if(squares[m.from] == Piece::W_KING){
        castlingrights.W_KingSide = false;
        castlingrights.W_QueenSide = false;
    }
    if(squares[m.from] == Piece::B_KING){
        castlingrights.B_KingSide = false;
        castlingrights.B_QueenSide = false;
    }

    if(m.from == 0 || m.to == 0) castlingrights.W_KingSide = false;
    if(m.from == 7 || m.to == 7) castlingrights.W_QueenSide = false;
    if(m.from == 56 || m.to == 56) castlingrights.B_KingSide = false;
    if(m.from == 63 || m.to == 63) castlingrights.B_QueenSide = false;
}


int Board::parseSquare(const std::string square){
    if(square.size() != 2){
        return -1;
    }

    char file = square[0];
    char rank = square[1];

    if(file < 'a' || file > 'h') return -1;
    if(rank < '1' || rank > '8') return -1;

    return int(file - 'a') + (int(rank - '1') * 8);
}


Move Board::parseMove(const std::string& Move){
    if(Move.size() != 4) return {-1, -1, EMPTY, EMPTY, 0};

    int start = parseSquare(Move.substr(0,2));
    int end = parseSquare(Move.substr(2,2));

    return {start, end, EMPTY, EMPTY, 0};
}

Move Board::findMatchingMove(const std::vector<Move>& legalMoves, const Move& inputMove){
    for(const auto& move : legalMoves){
        if(move.from == inputMove.from && move.to == inputMove.to){
            return move;
        }
    }
    return {-1, -1, EMPTY, EMPTY, 0};
}

bool Board::IsMoveLegal(const Move& move, const std::vector<Move>& legalMoves){
    for(const auto& legalMove : legalMoves){
        if(legalMove.from == move.from && legalMove.to == move.to){
            return true;
        }
    }
    return false;
}

/*
bool Board::isSquareAttacked(int square, bool byWhite) const{
    if(byWhite){
        if(onBoard(square + 7) && squares[square + 7] == W_PAWN) return true;
        if(onBoard(square + 9) && squares[square + 9] == W_PAWN) return true;
    }
    else{
        if(onBoard(square - 7) && squares[square - 7] == B_PAWN) return true;
        if(onBoard(square - 9) && squares[square - 9] == B_PAWN) return true;
    }

    int knightOffset[8] = {6, -6, 10, -10, 15, -15, 17, -17};
    for(int offset : knightOffset){
        int target = square + offset;
        if(onBoard(target)){
            int fileDiff = std::abs(file(target) - file(square));
            int rankDiff = std::abs(rank(target) - rank(square));
            if(!((fileDiff == 2 && rankDiff == 1) || (fileDiff == 1 && rankDiff == 2))) continue;
            
            if(squares[target] == W_KNIGHT && byWhite) return true;
            if(squares[target] == B_KNIGHT && !byWhite) return true;
        }
    }

    auto isFriendly = !byWhite
        ? [](int piece) { return piece == B_PAWN || piece == B_KNIGHT || piece == B_BISHOP || piece == B_ROOK || piece == B_QUEEN || piece == B_KING; }
        : [](int piece) { return piece == W_PAWN || piece == W_KNIGHT || piece == W_BISHOP || piece == W_ROOK || piece == W_QUEEN || piece == W_KING; };
    int direction[4] = {1, -1, 8 , -8};
    for(const auto& dir : direction){
        int target = square + dir;
        while(onBoard(target)){
            if((dir == -1 || dir == 1) && rank(target) != rank (square)) break;

            if(squares[target] == EMPTY){
                target += dir;
                continue;
            }
            if(isFriendly(squares[target])) break;
            else if(squares[target] == W_ROOK || squares[target] == W_QUEEN 
                    || squares[target] == B_ROOK || squares[target] == B_QUEEN) return true;
            else break;
        }
    }

    int direction1[4] = {9, -9, 7 , -7};
    for(const auto& dir : direction1){
        int target = square + dir;
        while(onBoard(target)){
            int fileDiff = std::abs(file(target) - file(target-dir));
            if(fileDiff != 1) break;

            if(squares[target] == EMPTY) {
                target += dir;
                continue;
            }
            if(isFriendly(squares[target])){
                break;
            }
            if(squares[target] == W_QUEEN || squares[target] == W_BISHOP 
               || squares[target] == B_QUEEN || squares[target] == B_BISHOP){
                return true;
            }
            else break;
        }
    }

    int offset[8] = {1, -1, 8, -8, 7, -7, 9, -9};
    for(const auto& dir : offset){
        int target = square + dir;
        if(onBoard(target)){
            if(byWhite && squares[target] == W_KING) return true;
            if(!byWhite && squares[target] == B_KING) return true;
        }
    }

    return false;

}
*/
// helper predicates you likely already have:
inline int fileOf(int sq) { return sq % 8; }
inline int rankOf(int sq) { return sq / 8; }
inline bool isWhitePiece(int p) { return p >= W_PAWN && p <= W_KING; }
inline bool isBlackPiece(int p) { return p >= B_PAWN && p <= B_KING; }

bool Board::isSquareAttacked(int square, bool byWhite) const{
    // 1) pawn attacks
    if(byWhite){
        int from = square - 7;
        if(onBoard(from) && fileOf(from) == fileOf(square) - 1 && squares[from] == W_PAWN) return true;
        from = square - 9;
        if(onBoard(from) && fileOf(from) == fileOf(square) + 1 && squares[from] == W_PAWN) return true;
    } 
    else{
        int from = square + 7;
        if(onBoard(from) && fileOf(from) == fileOf(square) + 1 && squares[from] == B_PAWN) return true;
        from = square + 9;
        if(onBoard(from) && fileOf(from) == fileOf(square) - 1 && squares[from] == B_PAWN) return true;
    }

    // 2) knights
    const int knightOffsets[8] = {6, -6, 10, -10, 15, -15, 17, -17};
    for(int d : knightOffsets){
        int t = square + d;
        if(!onBoard(t)) continue;
        int fd = std::abs(fileOf(t) - fileOf(square));
        int rd = std::abs(rankOf(t) - rankOf(square));
        if(!((fd == 1 && rd == 2) || (fd == 2 && rd == 1))) continue;
        if(byWhite & squares[t] == W_KNIGHT & squares[t] == B_KNIGHT) return true;
    }

    // helper to check attacker color
    auto isAttackerPiece = [&](int p){
        return byWhite ? isWhitePiece(p) : isBlackPiece(p);
    };

    // 3) straight rays (rook/queen)
    const int rookDirs[4] = {1, -1, 8, -8};
    for(int dir : rookDirs){
        int prev = square;
        int t = square + dir;
        while(onBoard(t)){
            // prevent horizontal wrap
            if((dir == 1 || dir == -1) && rankOf(t) != rankOf(prev)) break;

            int p = squares[t];
            if(p == EMPTY){
                prev = t;
                t += dir;
                continue;
            }
            // if piece belongs to attacker
            if(isAttackerPiece(p)){
                if ((byWhite && (p == W_ROOK || p == W_QUEEN)) ||
                    (!byWhite && (p == B_ROOK || p == B_QUEEN)) ){
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
        while(onBoard(t)){
            // ensure step is diagonal (file & rank step of 1)
            if(std::abs(fileOf(t) - fileOf(prev)) != 1 || std::abs(rankOf(t) - rankOf(prev)) != 1) break;

            int p = squares[t];
            if(p == EMPTY){
                prev = t;
                t += dir;
                continue;
            }
            if(isAttackerPiece(p)){
                if ((byWhite && (p == W_BISHOP || p == W_QUEEN)) ||
                    (!byWhite && (p == B_BISHOP || p == B_QUEEN))){
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
        int fd = std::abs(fileOf(t) - fileOf(square));
        int rd = std::abs(rankOf(t) - rankOf(square));
        if(fd <= 1 && rd <= 1){
            if (byWhite & squares[t] == W_KING & squares[t] == B_KING) return true;
        }
    }

    return false;
}

