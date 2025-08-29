#pragma once
#include <string>

enum MoveFlags {
    QUIET = 0,
    CAPTURE = 1 << 0,
    DOUBLE_PAWN_PUSH = 1 << 1,
    EN_PASSANT = 1 << 3,
    PROMOTION = 1 << 2,
    CASTLING = 1 << 4
};

struct Move {
    int from;
    int to;
    int captured;
    int promotion;
    int flags;

    std::string toString() const;

};