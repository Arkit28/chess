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
    playMove(b, "d2d3");   // white
    playMove(b, "e7e5");   // black
    playMove(b, "e2e4");   // white
    playMove(b, "d7d6");   // black
    REQUIRE( b.squares[11] == EMPTY );   // d2 empty
    REQUIRE( b.squares[19] == W_PAWN );  // d3 has white pawn
    REQUIRE( b.squares[12] == EMPTY );   // e2 empty
    REQUIRE( b.squares[28] == W_PAWN );  // e4 has white pawn
    REQUIRE( b.squares[52] == EMPTY );   // e7 empty
    REQUIRE( b.squares[36] == B_PAWN );  // e5 has black pawn
    REQUIRE( b.squares[51] == EMPTY );   // d7 empty
    REQUIRE( b.squares[43] == B_PAWN );  // d6 has black pawn

    SECTION( "knight piece moves correctly", "[piece]") {}
    SECTION( "rook piece moves correctly", "[piece]") {}
    SECTION( "bishop piece moves correctly", "[piece]") {}
    SECTION( "queen piece moves correctly", "[piece]") {}
    SECTION( "king piece moves correctly", "[piece]") {}
}