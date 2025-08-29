#pragma once

inline int rank(int square) {
    return square / 8;
}

inline int file(int square){
    return square % 8;
}

inline bool onBoard(int square) {
    return square >= 0 && square < 64;
}

inline char fileChar(int square) {
    return 'a' + (square % 8);
}

inline char rankChar(int square) {
    return '1' + (square / 8);
}

