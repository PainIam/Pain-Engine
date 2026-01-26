#include "test.h"


void runRookTest() {

    Board bd;
    bd.setFen("4k3/8/8/4p3/2p1R1p1/4p3/8/4K3 w - - 0 1");

    std::vector<Move> moves;
    MoveGen::moveGen(bd, moves);

    std::cout << "depth 1: " << bd.perft(1) << std::endl;
}

void runQueenTest() {

    Board bd;
    bd.setFen("4k3/8/8/8/3Q4/8/8/4K3 w - - 0 1");

    std::vector<Move> moves;
    MoveGen::moveGen(bd, moves);

    std::cout << "depth 1: " << bd.perft(1) << std::endl;
}

void runBishopTest() {
  
    Board bd;
    bd.setFen("4k3/8/8/8/3B4/8/8/4K3 w - - 0 1");

    std::vector<Move> moves;
    MoveGen::moveGen(bd, moves);

    std::cout << "depth 1: " << bd.perft(1) << std::endl;
}

void runKnightTest() {

    Board bd;
    bd.setFen("8/8/8/4n3/4N3/8/8/8 w - - 0 1");

    std::vector<Move> moves;
    MoveGen::moveGen(bd, moves);

    std::cout << "Knight Moves Found: " << moves.size() << " (Expected: 8)" << std::endl;
    if (moves.size() == 8) {
            std::cout << "Logic is FIXED. Now try perft(2)." << std::endl;
            std::cout << "Perft(2) Result: " << bd.perft(2) << " (Expected: 64)" << std::endl;
    }

}

void runPawnTest() {

    Board bd;
    bd.setFen("8/8/8/3p4/2P1P3/8/8/8 b - - 0 1");

    std::vector<Move> moves;
    MoveGen::moveGen(bd, moves);

    std::cout << "depth 1: " << bd.perft(1) << std::endl;
}