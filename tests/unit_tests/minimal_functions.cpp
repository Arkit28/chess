#include <src/core/board.hpp>

static void playMove(Board& b, const std::string& mv) {
    Move m = b.parseMove(mv, true);
    auto legal = b.generateLegalMoves();
    Move actual = b.findMatchingMove(legal, m);
    b.makeMove(actual);
    b.updateGameState(actual);
}