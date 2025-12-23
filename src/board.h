#pragma once
#include <iostream>

const int BOARD_SIZE = 128;

// side to move
enum side {BLACK_MOVE = -1, WHITE_MOVE = 1};

#define endl "\n"

enum class moveType {ORDINARY, KING_SIDE, QUEEN_SIDE, ENPASSANT, PROMO_QUEEN, PROMO_ROOK, PROMO_BISHOP, PROMO_KNIGHT};

enum class castling {NONE, SHORT, LONG, BOTH};

// enPassent square availability
const int NO_ENPAS = -1;

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
        void makeMove(const Move& move);
};