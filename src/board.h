#pragma once
#include <iostream>

#define BOARD_SIZE 128
#define WHITE_MOVE 1
#define BLACK_MOVE -1
#define INI_FEN "rnbqkbnr/ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define endl "\n"

enum Pieces {empty, wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, off};



class Board {
    private:
        int board[BOARD_SIZE];
        int m_toMove;
        int m_enPas;
        int m_Ply;
        int m_HisPly;
        int m_PosKey;
    public:
        Board();
        void print() const;
        std::string getFen();
};