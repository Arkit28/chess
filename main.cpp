#include "core\board.hpp"
#include "core\MoveGen.hpp"
#include "iostream"

auto toAlgebraic = [](int square) {
    char file = 'a' + (square % 8);
    char rank = '1' + (square / 8);
    return std::string{file, rank};
};

int main(){
    Board b;
    b.setStartPos();
    b.print();
    std::cout << "\n";
    std::cout << b.toFEN() << std::endl;

    b.squares[12] = EMPTY;
    b.squares[28] = W_PAWN;
    b.squares[27] = W_KNIGHT;
    b.squares[26] = B_ROOK;
    b.squares[25] = W_QUEEN;
    b.squares[1] = EMPTY;
    b.squares[2] = EMPTY;
    b.squares[3] = EMPTY;

    MoveGen mg;
    b.print();
    std::cout << "All legal moves:\n";
    for(auto& m : mg.GenPseudoLegal(b, true)){
            std::cout << toAlgebraic(m.from) << " to " << toAlgebraic(m.to) << " - " << m.flags << "\n";
        
    }

    
}