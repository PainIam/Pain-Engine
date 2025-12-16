#pragma once


struct Move {
    int pieceType;
    int fromSq;
    int toSq;
    int capture;
    int moveType;

    Move(int piece, int from, int to, int cap, int type) : // initializer list
    pieceType(piece),
    fromSq(from),
    toSq(to),
    capture(cap),
    moveType(type) {

    }
};


