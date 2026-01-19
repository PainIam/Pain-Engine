#pragma once
#include "board.h"
#include <vector>

struct MoveGen {

    static void moveGen(const Board& bd, std::vector<Move>& moves);
    static void genKnightMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq);
    static void genRookMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq);
    static void genBishopMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq);
    static void genQueenMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq);
    static void genKingMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq);

    static bool isFriendly(int piece, int myColor);
    static bool isEnemy(int piece, int myColor);

};