#include "board.h"



Board::Board() {
    // create temp variable which should nicely initialize board
    int start[BOARD_SIZE] = {
    brook, bknight, bbishop, bqueen, bking, bbishop, bknight, brook, off, off, off, off, off, off, off, off,
    bpawn, bpawn, bpawn, bpawn, bpawn, bpawn, bpawn, bpawn, off, off, off, off, off, off, off, off,
    empty, empty, empty, empty, empty, empty, empty, empty, off, off, off, off, off, off, off, off,
    empty, empty, empty, empty, empty, empty, empty, empty, off, off, off, off, off, off, off, off,
    empty, empty, empty, empty, empty, empty, empty, empty, off, off, off, off, off, off, off, off,
    empty, empty, empty, empty, empty, empty, empty, empty, off, off, off, off, off, off, off, off,
    wpawn, wpawn, wpawn, wpawn, wpawn, wpawn, wpawn, wpawn, off, off, off, off, off, off, off, off,
    wrook, wknight, wbishop, wqueen, wking, wbishop, wknight, wrook, off, off, off, off, off, off, off, off
    };

    // initialize board with start
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = start[i];
    }

}

void Board::print() const {
    std::string pieces {".PRNBQKprnbqk"}; // string pieces to map appropriate piece with enum --see board.h

    std::cout << endl;
    for (int rank = 0; rank < 8; rank++) {
        std::cout << 8 - rank << "  "; 
        for (int file = 0; file < 16; file++) {
            int square = (rank * 16) + file; // square variable

            if ((square & 0x88) == 0)
                std::cout << pieces[board[square]] << " ";
        }
        std::cout << endl;
    }
    std::cout << "   a b c d e f g h" << endl;

}