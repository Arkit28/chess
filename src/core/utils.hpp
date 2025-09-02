#pragma once

inline int rank(int square) {
    return square / 8;
}

inline int file(int square){
    return square % 8;
}

inline bool onBoard(const int square) {
    return square >= 0 && square < 64;
}

inline char fileChar(int square) {
    return 'a' + (square % 8);
}

inline char rankChar(int square) {
    return '1' + (square / 8);
}

inline bool ValidRank(const int square){
    return (rank(square) < 8 && rank(square) >= 0 && file(square) < 8 && file(square) >= 0);
}

