#include "board.h"
#include "move.h"

int main() {

    Board myBoard;
    myBoard.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    long nodes = myBoard.perft(1);
    std::cout << "Depth 1: " << nodes << " nodes" << "\n";
}