#pragma once
#include <iostream>

#define BOARD_SIZE 128

// side to move
#define WHITE_MOVE 1
#define BLACK_MOVE -1

// for debugging
#define INI_FEN "rnbqkbnr/ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// self explainatory
#define endl "\n"

// castling availability
#define NO_CASTLE 0
#define SHORT_CASTLE 1
#define LONG_CASTLE 2
#define BOTH_CASTLE 3

// castling direction
#define CASTLE_SHORT 1
#define CASTLE_LONG 2

// enPassent square availability
#define NO_ENPAS -1 // cannot be a valid square

enum Pieces {empty, wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, off};



class Board {
    private:
        int board[BOARD_SIZE];
        int m_toMove;
        int m_enPas;
        int m_WhiteCastle;
        int m_BlackCastle;
        int m_halfMoves;
        int m_fullMoves;
    public:
        Board();
        void print() const;
        std::string getFen();
        void setFen(const std::string&);
        void makeMove(Move& move);
};