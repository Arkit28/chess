#include "core/board.hpp"
#include "core/moveGen.hpp"
#include "engine/engine.hpp"
#include "engine/piece_tables.hpp"
#include <iostream>
#include <string>
#include <vector>


enum class GameMode {
    Player_v_Player,
    Player_v_Engine,
    Engine_v_Player
};



class ChessGame {
public:
    ChessGame() : mode_(GameMode::Player_v_Player), engine_(nullptr) {
        board.setStartPos();
        gameHistory.push_back(board.toFEN());
    }

    ~ChessGame(){
        delete engine_;
    }

    void selectGameMode(){
        std::cout << "Select game mode:\n";
        std::cout << "1. Player vs Player\n";
        std::cout << "2. Player vs Engine (Player is white)\n";
        std::cout << "3. Engine vs Player (Engine is white)\n";
        std::cout << "Enter choice 1-3: ";

        int choice;
        std::cin >> choice;

        switch (choice){
            case 1: 
                mode_ = GameMode::Player_v_Player;
                break;
            case 2:
                mode_ = GameMode::Player_v_Engine;
                setUpEngine();
                break;
            case 3:
                mode_ = GameMode::Engine_v_Player;
                setUpEngine();
                break;
            default:
                std::cout << "invalid choice, defaulting to Player v Player\n";
                mode_ = GameMode::Player_v_Player;
        }
    }

    void playGame(){

        selectGameMode();

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

            bool moveSuccess = false;
            //whos move is it
            bool isEngineMove = (mode_ == GameMode::Player_v_Engine && !board.whiteToMove) ||
                                (mode_ == GameMode:: Engine_v_Player && board.whiteToMove);

            if(isEngineMove){
                moveSuccess = handleEngineMove();
            }
            else{
                moveSuccess = handlePlayerMove();
            }

            if(moveSuccess){
                gameHistory.push_back(board.toFEN());
            }
        }
    }

private:
    Board board;
    std::vector<std::string> gameHistory;
    GameMode mode_;
    ChessEngine* engine_;

    void displayGameState(){

        std::cout << "\n" << "==========================" << "\n\n\n";
        board.print(board.whiteToMove);
        std::cout << "\n FEN: " << board.toFEN() << "\n";

        if(board.isCheck(board.whiteToMove)){
            std::cout << "*** " << (board.whiteToMove ? "White" : "Black") << " is in check ***\n";
        }
        std::cout << "\n";
    }

    void setUpEngine(){
        std::cout << "Select engine difficulty: \n";
        std::cout << "0. Random moves\n";
        std::cout << "1. Beginner\n";
        std::cout << "2. Easy\n";
        std::cout << "3. Medium\n";
        std::cout << "4. Hard\n";
        std::cout << "5. Expert\n";
        std::cout << "Enter choice (0-5): ";

        int level;
        std::cin >> level;

        engine_ = new ChessEngine(static_cast<EngineLevel>(level));
    }


    bool handleEngineMove() {
        std::cout << "Computer is thinking how to cook ur ass..\n";

        auto start = std::chrono::steady_clock::now();
        Move engineMove = engine_->getBestMove(board);
        auto end = std::chrono::steady_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        if(engineMove.from == -1){
            std::cout << "Engine error: No move found\n";
            return false;
        }

        board.makeMove(engineMove);
        board.updateGameState(engineMove);

        std::cout << "Computer played: " << engineMove.toString();
        std::cout << "\n(searched " << engine_->getNodesSearched() << " nodes in " << duration.count() << "ms)\n";
        
        return true;
    }

    bool handlePlayerMove(){
        std::string input;
        std::cout << (board.whiteToMove ? "White" : "Black") << " to move: ";
        std::cin >> input;

        if(input == "quit" || input == "exit"){
            std::cout << "Game ended.\n";
            exit(0);
        }
        else if(input == "resign"){
            std::cout << (board.whiteToMove ? "White" : "Black") << " has resigned. " << (board.whiteToMove ? "Black" : "White") << " wins!\n";
            exit(0); 
        }


        Move inputMove = board.parseMove(input, true);
        std::cout << "parsed: from= " << inputMove.from << "to= " << inputMove.to << "\n";
        if(inputMove.from == -1){
            std::cout << "Invalid move format. \n";
            return false;
        }

        std::vector<Move>legalMoves = board.generateLegalMoves();
        Move actualMove = board.findMatchingMove(legalMoves, inputMove);
        if(actualMove.from == -1){
            std::cout << "Illegal Move!\n";
            return false;
        }

        board.makeMove(actualMove);
        board.updateGameState(actualMove);
        return true;

    }

    void showLegalMoves(){
        std::vector<Move> legalMoves = board.generateLegalMoves();
        std::cout << "Legal moves (" << legalMoves.size() << "): ";
        for(size_t i = 0; i < legalMoves.size(); i++){
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