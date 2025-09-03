#include "core/board.hpp"
#include "core/moveGen.hpp"
#include <iostream>
#include <string>
#include <vector>

class ChessGame {


public:
    ChessGame(){
        board.setStartPos();
        gameHistory.push_back(board.toFEN());
    }

    void playGame(){
        std::cout << "Game Started! Enter moves in format e2e4\n";
        std::cout << "Type 'quit' to exit, 'draw?' to offer a draw or 'resign' to resign\n\n";

        while(true){
            displayGameState();

            if(board.isCheckmate()){
                std::cout << "Checkmate!" << (board.whiteToMove ? "Black" : "White") << " wins\n"; 
                break;
            }

            if(board.isStalemate()){
                std::cout << "Stalemate! Game drawn\n";
                break;
            }

            std::string input;
            std::cout << (board.whiteToMove ? "White" : "Black") << " to move: ";
            std::cin >> input;
            
            if(input == "quit" || input == "exit"){
                std::cout << "Game ended.\n";
                break;
            }
            else if(input == "resign"){
                std::cout << (board.whiteToMove ? "White" : "Black") << " has resigned. " << (board.whiteToMove ? "Black" : "White") << " wins\n";
                break;
            }
            else if(input == "draw?"){
                std::cout << "Game drawn\n";
            }
            else if(input == "moves"){
                showLegalMoves();
                continue;
            }

            if(makeUserMove(input)){
                gameHistory.push_back(board.toFEN());
            }
            else {
                std::cout << "Invalid move. Try again. \n";
            }
        }
    }

private:
    Board board;
    std::vector<std::string> gameHistory;

    void displayGameState(){
        std::cout << "\n" << "==========================" << "\n";
        board.print(); 
        std::cout << "\n FEN: " << board.toFEN() << "\n";

    if(board.isCheck(board.whiteToMove)){
        std::cout << "*** " << (board.whiteToMove ? "White" : "Black") << " is in check ***\n";
    }
    std::cout << "\n";
    }

    bool makeUserMove(const std::string& moveStr){
        Move inputMove = board.parseMove(moveStr);
        if(inputMove.from == -1){
            std::cout << "Invalid move format. Format is e2e4, a2b3 etc\n";
            return false;
        }

        std::vector<Move> legalMoves = board.generateLegalMoves();
        Move actualMove = board.findMatchingMove(legalMoves, inputMove);
        if(actualMove.from == -1){
            std::cout << "Illegal Move!\n";
            return false;
        }

        board.makeMove(actualMove);
        board.updateGameState(actualMove);

        std::cout << "Move played: " << actualMove.toString() << "\n"; 
        return true;
    }

    void showLegalMoves(){
        std::vector<Move> legalMoves = board.generateLegalMoves();
        std::cout << "Legal moves (" << legalMoves.size() << "): ";
        for(size_t i = 0; 0 < legalMoves.size(); i++){
            std::cout << legalMoves[i].toString();
            if(i < legalMoves.size()-1){
                std::cout << ", ";
            }
            if((i+1) % 10 == 0){
                std::cout << "\n";
            }
        }
        std::cout << "\n\n";
    }

};

int main(){
    ChessGame game;
    game.playGame();
}