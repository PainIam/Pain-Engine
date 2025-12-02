#pragma once
#include <iostream>

#define BOARD_SIZE 128
#define endl "\n"

enum {empty, wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, off};


class Board {
    private:
        int board[BOARD_SIZE];
    public:
        Board();
        void print() const;
};