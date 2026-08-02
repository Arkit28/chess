#include <catch2/catch_test_macros.hpp>
#include "src/core/board.hpp"

// Test 1: initial board setup
// Test 2: board reset after some moves are made
// Test 3: General FEN representation is correct, pre and post move 



TEST_CASE( "board initial setup adheres to standard chess rules", "[board]" ) {
    
    Board testBoard;
    testBoard.setStartPos();

    REQUIRE( testBoard.squares[0] == Piece::W_ROOK    );
    REQUIRE( testBoard.squares[1] == Piece::W_KNIGHT  );
    REQUIRE( testBoard.squares[2] == Piece::W_BISHOP  );
    REQUIRE( testBoard.squares[3] == Piece::W_QUEEN   );
    REQUIRE( testBoard.squares[4] == Piece::W_KING    );
    REQUIRE( testBoard.squares[5] == Piece::W_BISHOP  );
    REQUIRE( testBoard.squares[6] == Piece::W_KNIGHT  );
    REQUIRE( testBoard.squares[7] == Piece::W_ROOK    );
    REQUIRE( testBoard.squares[8] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[9] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[10] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[11] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[12] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[13] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[14] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[15] == Piece::W_PAWN   );
    //black pieces
    REQUIRE( testBoard.squares[48] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[49] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[50] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[51] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[52] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[53] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[54] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[55] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[56] == Piece::B_ROOK   );
    REQUIRE( testBoard.squares[57] == Piece::B_KNIGHT );
    REQUIRE( testBoard.squares[58] == Piece::B_BISHOP );
    REQUIRE( testBoard.squares[59] == Piece::B_QUEEN  );
    REQUIRE( testBoard.squares[60] == Piece::B_KING   );
    REQUIRE( testBoard.squares[61] == Piece::B_BISHOP );
    REQUIRE( testBoard.squares[62] == Piece::B_KNIGHT );
    REQUIRE( testBoard.squares[63] == Piece::B_ROOK   );

    
SECTION( "board is reset to correct initial position", "[board]") {

    testBoard.squares[0] = EMPTY; 
    testBoard.squares[2] = W_KING;
    testBoard.squares[19] = B_QUEEN;
    testBoard.squares[17] = W_PAWN;
    testBoard.squares[9] = EMPTY;
    testBoard.squares[58] = EMPTY;
    testBoard.squares[64] = W_BISHOP;
    testBoard.squares[34] = B_PAWN;
    testBoard.squares[45] = W_KNIGHT;
    testBoard.squares[22] = B_KNIGHT;
    testBoard.squares[7] = EMPTY;
    testBoard.squares[28] = W_ROOK;
    testBoard.squares[8] = EMPTY;
    testBoard.squares[10] = EMPTY;
    testBoard.squares[49] = EMPTY;
    testBoard.squares[50] = EMPTY;
    testBoard.squares[51] = EMPTY;

    testBoard.setStartPos();

    REQUIRE( testBoard.squares[0] == Piece::W_ROOK    );
    REQUIRE( testBoard.squares[1] == Piece::W_KNIGHT  );
    REQUIRE( testBoard.squares[2] == Piece::W_BISHOP  );
    REQUIRE( testBoard.squares[3] == Piece::W_QUEEN   );
    REQUIRE( testBoard.squares[4] == Piece::W_KING    );
    REQUIRE( testBoard.squares[5] == Piece::W_BISHOP  );
    REQUIRE( testBoard.squares[6] == Piece::W_KNIGHT  );
    REQUIRE( testBoard.squares[7] == Piece::W_ROOK    );
    REQUIRE( testBoard.squares[8] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[9] == Piece::W_PAWN    );
    REQUIRE( testBoard.squares[10] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[11] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[12] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[13] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[14] == Piece::W_PAWN   );
    REQUIRE( testBoard.squares[15] == Piece::W_PAWN   );
    //black pieces
    REQUIRE( testBoard.squares[48] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[49] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[50] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[51] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[52] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[53] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[54] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[55] == Piece::B_PAWN   );
    REQUIRE( testBoard.squares[56] == Piece::B_ROOK   );
    REQUIRE( testBoard.squares[57] == Piece::B_KNIGHT );
    REQUIRE( testBoard.squares[58] == Piece::B_BISHOP );
    REQUIRE( testBoard.squares[59] == Piece::B_QUEEN  );
    REQUIRE( testBoard.squares[60] == Piece::B_KING   );
    REQUIRE( testBoard.squares[61] == Piece::B_BISHOP );
    REQUIRE( testBoard.squares[62] == Piece::B_KNIGHT );
    REQUIRE( testBoard.squares[63] == Piece::B_ROOK   );

}

SECTION( "board's FEN is correct for pieces", "[board]" ) {

    testBoard.setStartPos();
    std::string preFEN = testBoard.toFEN();
    REQUIRE( preFEN == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    testBoard.squares[0] = EMPTY;
    testBoard.squares[28] = W_ROOK;
    testBoard.squares[34] = B_ROOK;

    std::string postFEN = testBoard.toFEN();
    REQUIRE( postFEN == "rnbqkbnr/pppppppp/8/2r5/4R3/8/PPPPPPPP/1NBQKBNR w Kkq - 0 1");

    

}


}
