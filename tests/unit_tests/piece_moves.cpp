#include <catch2/catch_test_macros.hpp>
#include "board.hpp"
#include "minimal_functions.cpp"

TEST_CASE( "pawn move updates correct squares", "[moves]" ) {
    Board b;
    b.setStartPos();

}


TEST_CASE( "pawn piece moves correctly" , "[piece]") {

    Board b;
    b.setStartPos();
    playMove(b, "d2d3");
    playMove(b, "e7e5");
    playMove(b, "e2e4");
    playMove(b, "d7d6");

    REQUIRE( b.squares[D2] == EMPTY );
    REQUIRE( b.squares[D3] == W_PAWN );
    REQUIRE( b.squares[E2] == EMPTY );
    REQUIRE( b.squares[E4] == W_PAWN );
    REQUIRE( b.squares[E7] == EMPTY );
    REQUIRE( b.squares[E5] == B_PAWN );
    REQUIRE( b.squares[D7] == EMPTY );
    REQUIRE( b.squares[D6] == B_PAWN );

}

TEST_CASE( "knight piece moves correctly", "[piece]") {

    Board b;
    b.setStartPos();
    playMove(b, "b1c3");
    playMove(b, "b8c6");
    playMove(b, "g1f3");
    playMove(b, "g8f6");

    REQUIRE( b.squares[B1] == EMPTY );
    REQUIRE( b.squares[G1] == EMPTY );
    REQUIRE( b.squares[B8] == EMPTY );
    REQUIRE( b.squares[G8] == EMPTY );
    REQUIRE( b.squares[C6] == B_KNIGHT );
    REQUIRE( b.squares[F6] == B_KNIGHT );
    REQUIRE( b.squares[C3] == W_KNIGHT );
    REQUIRE( b.squares[F3] == W_KNIGHT );
}

TEST_CASE( "rook piece moves correctly", "[piece]") {

    Board b;
    b.setStartPos();
    playMove(b, "d2d4");
    playMove(b, "d7d5");
    playMove(b, "d1d2");
    playMove(b, "d8d7");
    playMove(b, "a2a4");
    playMove(b, "a7a5");
    playMove(b, "h2h4");
    playMove(b, "h7h5");
    playMove(b, "e2e4");
    playMove(b, "f7f5");
    playMove(b, "g1f3");
    playMove(b, "g8f6");
    playMove(b, "f1e2");
    playMove(b, "f8e7");
    playMove(b, "g1f3");
    playMove(b, "g8f6");
    playMove(b, "f3e5");
    playMove(b, "f6g8");
    playMove(b, "b2b4");
    playMove(b, "g7g6");
    playMove(b, "a1a3");
    playMove(b, "a8a6");
    REQUIRE( b.squares[A1] == EMPTY );
    REQUIRE( b.squares[A3] == W_ROOK );
    REQUIRE( b.squares[A8] == EMPTY );
    REQUIRE( b.squares[A6] == B_ROOK );
    playMove(b, "a3h3");
    playMove(b, "a6a7");
    REQUIRE( b.squares[A3] == EMPTY );
    REQUIRE( b.squares[H3] == W_ROOK );
    REQUIRE( b.squares[A6] == EMPTY );
    REQUIRE( b.squares[A7] == B_ROOK );
    playMove(b, "h3h1");
    playMove(b, "a7a8");
    REQUIRE( b.squares[A3] == EMPTY );
    REQUIRE( b.squares[H1] == W_ROOK );
    REQUIRE( b.squares[A7] == EMPTY );
    REQUIRE( b.squares[A8] == B_ROOK );
    playMove(b, "h1f1");
    playMove(b, "a8a6");
    REQUIRE( b.squares[H1] == EMPTY );
    REQUIRE( b.squares[F1] == W_ROOK );
    REQUIRE( b.squares[A8] == EMPTY );
    REQUIRE( b.squares[A6] == B_ROOK );

    REQUIRE( b.squares[A1] == EMPTY  );
    REQUIRE( b.squares[F1] == W_ROOK );
    REQUIRE( b.squares[A8] == EMPTY  );
    REQUIRE( b.squares[A6] == B_ROOK );
}

TEST_CASE( "bishop piece moves correctly", "[piece]") {

    Board b;
    b.setStartPos();
    playMove(b, "c2c4");
    playMove(b, "c7c5");
    playMove(b, "d2d3");
    playMove(b, "d7d5");
    playMove(b, "c1d2");
    playMove(b, "c8d7");
    REQUIRE( b.squares[C1] == EMPTY );
    REQUIRE( b.squares[D2] == W_BISHOP );
    REQUIRE( b.squares[C8] == EMPTY );
    REQUIRE( b.squares[D7] == B_BISHOP );
    playMove(b, "d2c3");
    playMove(b, "d7c6");
    REQUIRE( b.squares[D2] == EMPTY );
    REQUIRE( b.squares[C3] == W_BISHOP );
    REQUIRE( b.squares[D7] == EMPTY );
    REQUIRE( b.squares[C6] == B_BISHOP );
    playMove(b, "c3d2");
    playMove(b, "c6d7");
    REQUIRE( b.squares[C3] == EMPTY );
    REQUIRE( b.squares[D2] == W_BISHOP );
    REQUIRE( b.squares[C6] == EMPTY );
    REQUIRE( b.squares[D7] == B_BISHOP );
    playMove(b, "d2c1");
    playMove(b, "d7c8");
    REQUIRE( b.squares[D2] == EMPTY );
    REQUIRE( b.squares[C1] == W_BISHOP );
    REQUIRE( b.squares[D7] == EMPTY );
    REQUIRE( b.squares[C8] == B_BISHOP );

    REQUIRE( b.squares[C1] == W_BISHOP );
    REQUIRE( b.squares[C8] == B_BISHOP );
}

TEST_CASE( "queen piece moves correctly", "[piece]") {

    Board b;
    b.setStartPos();
    playMove(b, "d2d4");
    playMove(b, "d7d5");
    playMove(b, "e2e3");
    playMove(b, "e7e5");
    playMove(b, "d1d2");
    playMove(b, "d8d7");
    REQUIRE( b.squares[D1] == EMPTY );
    REQUIRE( b.squares[D2] == W_QUEEN );
    REQUIRE( b.squares[D8] == EMPTY );
    REQUIRE( b.squares[D7] == B_QUEEN );
    playMove(b, "d2e2");
    playMove(b, "d7e7");
    REQUIRE( b.squares[D2] == EMPTY );
    REQUIRE( b.squares[E2] == W_QUEEN );
    REQUIRE( b.squares[D7] == EMPTY );
    REQUIRE( b.squares[E7] == B_QUEEN );
    playMove(b, "e2d1");
    playMove(b, "e7e6");
    REQUIRE( b.squares[E2] == EMPTY );
    REQUIRE( b.squares[D1] == W_QUEEN );
    REQUIRE( b.squares[E7] == EMPTY );
    REQUIRE( b.squares[E6] == B_QUEEN );
    playMove(b, "d1e1");
    playMove(b, "e6d6");
    playMove(b, "e1e2");
    playMove(b, "d6e6");
    playMove(b, "e2d3");
    playMove(b, "e6d7");
    playMove(b, "d3e2");
    playMove(b, "d7e6");
    playMove(b, "e2d1");
    playMove(b, "e6d7");

    REQUIRE( b.squares[D1] == W_QUEEN );
    REQUIRE( b.squares[D7] == B_QUEEN );
}

TEST_CASE( "king piece moves correctly", "[piece]") {

    Board b;
    b.setStartPos();
    playMove(b, "e2e4");
    playMove(b, "e7e5");
    playMove(b, "d2d3");
    playMove(b, "d7d6");
    playMove(b, "f2f3");
    playMove(b, "f7f6");
    playMove(b, "g2g3");
    playMove(b, "g7g6");
    playMove(b, "a2a3");
    playMove(b, "f8g7");
    playMove(b, "f1g2");
    playMove(b, "a7a6");
    playMove(b, "e1e2");
    playMove(b, "e8e7");
    REQUIRE( b.squares[E1] == EMPTY );
    REQUIRE( b.squares[E2] == W_KING );
    REQUIRE( b.squares[E8] == EMPTY );
    REQUIRE( b.squares[E7] == B_KING );
    playMove(b, "e2f2");
    playMove(b, "e7f7");
    REQUIRE( b.squares[E2] == EMPTY );
    REQUIRE( b.squares[F2] == W_KING );
    REQUIRE( b.squares[E7] == EMPTY );
    REQUIRE( b.squares[F7] == B_KING );
    playMove(b, "f2f1");
    playMove(b, "f7f8");
    REQUIRE( b.squares[F2] == EMPTY );
    REQUIRE( b.squares[F1] == W_KING );
    REQUIRE( b.squares[F7] == EMPTY );
    REQUIRE( b.squares[F8] == B_KING );
    playMove(b, "f1e1");
    playMove(b, "f8e8");
    playMove(b, "e1f2");
    playMove(b, "e8f7");
    playMove(b, "f2e3");
    playMove(b, "f7e6");
    playMove(b, "e3f2");
    playMove(b, "e6f7");
    playMove(b, "f2e1");
    playMove(b, "f7e8");

    REQUIRE( b.squares[E1] == W_KING );
    REQUIRE( b.squares[E8] == B_KING );
}
