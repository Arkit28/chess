#include <catch2/catch_test_macros.hpp>
#include "src/core/board.hpp"



// Test 1: initial board setup
// Test 2: board reset after some moves are made
// Test 3: General FEN representation is correct, pre and post move


TEST_CASE( "board initial setup adheres to standard chess rules", "[board]" ) {

    Board testBoard;
    testBoard.setStartPos();

    REQUIRE( testBoard.squares[A1] == Piece::W_ROOK    );
    REQUIRE( testBoard.squares[B1] == Piece::W_KNIGHT  );
    REQUIRE( testBoard.squares[C1] == Piece::W_BISHOP  );
    REQUIRE( testBoard.squares[D1] == Piece::W_QUEEN   );
    REQUIRE( testBoard.squares[E1] == Piece::W_KING    );
    REQUIRE( testBoard.squares[F1] == Piece::W_BISHOP  );
    REQUIRE( testBoard.squares[G1] == Piece::W_KNIGHT  );
    REQUIRE( testBoard.squares[H1] == Piece::W_ROOK    );
    REQUIRE( testBoard.squares[A2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[B2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[C2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[D2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[E2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[F2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[G2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[H2] == Piece::W_PAWN    );
    //black pieces
    REQUIRE( testBoard.squares[A7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[B7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[C7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[D7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[E7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[F7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[G7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[H7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[A8] == Piece::B_ROOK    );
    REQUIRE( testBoard.squares[B8] == Piece::B_KNIGHT  );
    REQUIRE( testBoard.squares[C8] == Piece::B_BISHOP  );
    REQUIRE( testBoard.squares[D8] == Piece::B_QUEEN   );
    REQUIRE( testBoard.squares[E8] == Piece::B_KING    );
    REQUIRE( testBoard.squares[F8] == Piece::B_BISHOP  );
    REQUIRE( testBoard.squares[G8] == Piece::B_KNIGHT  );
    REQUIRE( testBoard.squares[H8] == Piece::B_ROOK    );


SECTION( "board is reset to correct initial position", "[board]") {

    testBoard.squares[A1] = EMPTY;
    testBoard.squares[C1] = W_KING;
    testBoard.squares[D3] = B_QUEEN;
    testBoard.squares[B3] = W_PAWN;
    testBoard.squares[B2] = EMPTY;
    testBoard.squares[C8] = EMPTY;
    testBoard.squares[64] = W_BISHOP;
    testBoard.squares[C5] = B_PAWN;
    testBoard.squares[F6] = W_KNIGHT;
    testBoard.squares[G3] = B_KNIGHT;
    testBoard.squares[H1] = EMPTY;
    testBoard.squares[E4] = W_ROOK;
    testBoard.squares[A2] = EMPTY;
    testBoard.squares[C2] = EMPTY;
    testBoard.squares[B7] = EMPTY;
    testBoard.squares[C7] = EMPTY;
    testBoard.squares[D7] = EMPTY;

    testBoard.setStartPos();

    REQUIRE( testBoard.squares[A1] == Piece::W_ROOK    );
    REQUIRE( testBoard.squares[B1] == Piece::W_KNIGHT  );
    REQUIRE( testBoard.squares[C1] == Piece::W_BISHOP  );
    REQUIRE( testBoard.squares[D1] == Piece::W_QUEEN   );
    REQUIRE( testBoard.squares[E1] == Piece::W_KING    );
    REQUIRE( testBoard.squares[F1] == Piece::W_BISHOP  );
    REQUIRE( testBoard.squares[G1] == Piece::W_KNIGHT  );
    REQUIRE( testBoard.squares[H1] == Piece::W_ROOK    );
    REQUIRE( testBoard.squares[A2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[B2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[C2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[D2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[E2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[F2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[G2] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[H2] == Piece::W_PAWN    );
    //black pieces
    REQUIRE( testBoard.squares[A7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[B7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[C7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[D7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[E7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[F7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[G7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[H7] == Piece::B_PAWN    );
    REQUIRE( testBoard.squares[A8] == Piece::B_ROOK    );
    REQUIRE( testBoard.squares[B8] == Piece::B_KNIGHT  );
    REQUIRE( testBoard.squares[C8] == Piece::B_BISHOP  );
    REQUIRE( testBoard.squares[D8] == Piece::B_QUEEN   );
    REQUIRE( testBoard.squares[E8] == Piece::B_KING    );
    REQUIRE( testBoard.squares[F8] == Piece::B_BISHOP  );
    REQUIRE( testBoard.squares[G8] == Piece::B_KNIGHT  );
    REQUIRE( testBoard.squares[H8] == Piece::B_ROOK    );

}

SECTION( "board's FEN is correct for pieces", "[board]" ) {

    testBoard.setStartPos();
    std::string preFEN = testBoard.toFEN();
    REQUIRE( preFEN == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    testBoard.squares[A1] = EMPTY;
    testBoard.squares[E4] = W_ROOK;
    testBoard.squares[C5] = B_ROOK;

    std::string postFEN = testBoard.toFEN();
    REQUIRE( postFEN == "rnbqkbnr/pppppppp/8/2r5/4R3/8/PPPPPPPP/1NBQKBNR w Kkq - 0 1");

}


}
