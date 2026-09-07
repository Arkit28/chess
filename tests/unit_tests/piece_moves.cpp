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

    TEST_CASE( "rook piece moves correctly", "[piece]") {}
    TEST_CASE( "bishop piece moves correctly", "[piece]") {}
    TEST_CASE( "queen piece moves correctly", "[piece]") {}
    TEST_CASE( "king piece moves correctly", "[piece]") {}
