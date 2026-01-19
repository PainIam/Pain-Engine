#pragma once
#include "move.h"
#include <iostream>
#include <array>
#include <vector>

const int BOARD_SIZE = 128;
const int NO_ENPAS = -1;

enum class moveType {ORDINARY, KING_SIDE, QUEEN_SIDE, ENPASSANT, PROMO_QUEEN, PROMO_ROOK, PROMO_BISHOP, PROMO_KNIGHT};
enum class castling {NONE, SHORT, LONG, BOTH};
enum Pieces {EMPTY = 0,
    WPAWN = 1, WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING,
    BPAWN = 7, BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING,
    INVALID = 99};
enum side {BLACK_MOVE = -1, WHITE_MOVE = 1};

struct MoveGen;

class Board {

    friend struct MoveGen;

    private:
        std::array<int, BOARD_SIZE> board;
        std::vector<undoInfo> history;
        int m_toMove;
        int m_enPas;
        int m_WhiteCastle;
        int m_BlackCastle;
        int m_halfMoves;
        int m_fullMoves;
    public:
        Board();
        void print() const;
        std::string getFen() const;
        void setFen(const std::string&);
        void makeMove(const Move& move, undoInfo& undo);
        void unMakeMove(const Move& move);
        long perft(int depth);
        int findKingSquare(int side);
        std::vector<Move> generateMoves();
};