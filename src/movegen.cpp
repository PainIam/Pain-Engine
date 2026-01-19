#include "movegen.h"
#include "move.h"


void MoveGen::moveGen(const Board& bd, std::vector<Move>& moves) {

    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((0x88 & i) == 0) {
            int piece = bd.board[i];
            
            // if sq is not empty
            if (piece != EMPTY) {
                
                // check if piece is friendly.
                if (isFriendly(piece, bd.m_toMove)) {
                    
                    // IFF piece is friendly shall we generate moves  for it
                    if (piece == WKNIGHT || piece == BKNIGHT)
                        MoveGen::genKnightMoves(bd, piece , moves, i);   
                    if (piece == WROOK || piece == BROOK)
                        MoveGen::genRookMoves(bd, piece, moves, i);
                    if (piece == WBISHOP || piece == BBISHOP)
                        MoveGen::genBishopMoves(bd, piece, moves, i);
                    if (piece == WQUEEN || piece == BQUEEN)
                        MoveGen::genQueenMoves(bd, piece, moves, i);

                        
                }
            
            }
        }
    }
}

void MoveGen::genKnightMoves(const Board& bd, int piece,std::vector<Move>& moves, int sq) {

    std::array<int, 8> knightOffsets = {-33, -31, -18, -14, 14, 18, 31, 33};

    // loop through the offsets
    for (const auto offset : knightOffsets) {

        // ini sq + offset equals our target sq
        auto target = offset + sq;
        if ((0x88 & target) == 0) {

            int targetPiece = bd.board[target];

            // if the piece is not friendly then i can go there
            if (!isFriendly(targetPiece, bd.m_toMove))
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));
        }
    }
}

void MoveGen::genRookMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq) {

    std::array<int, 4> rookOffsets {-16, -1, 1, 16};

    for (const auto offset : rookOffsets) {

        auto target = offset + sq;
        while((0x88 & target) == 0) {

            int targetPiece = bd.board[target];

            if (!isFriendly(targetPiece, bd.m_toMove)) {
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));
                if (targetPiece == EMPTY) {
                    target += offset;
                    continue;
                } else if (isEnemy(targetPiece, bd.m_toMove)) {
                     break;
                }

            }   

            if (isFriendly(targetPiece, bd.m_toMove))
                break;
            
            target += offset;
        }
    }
}

void MoveGen::genBishopMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq) {

    std::array<int, 4> bishopOffsets {-17, -15, 15, 17};

    for (const auto offset : bishopOffsets) {
        
        auto target = sq + offset;

        while ((0x88 & target) == 0) {
            
            int targetPiece = bd.board[target];

            if (!isFriendly(targetPiece, bd.m_toMove)) {
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));
                if (targetPiece == EMPTY) {
                    target += offset;
                    continue;
                } else if (isEnemy(targetPiece, bd.m_toMove)) {
                    break;
                }
            }

            if (isFriendly(targetPiece, bd.m_toMove))
                break;
            
        }
    }
}

void MoveGen::genQueenMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq) {

    std::array<int, 8> queenOffsets {-17, -16, -15, -1, 1, 15, 16, 17};

    for (const auto offset : queenOffsets) {
        auto target = sq + offset;

        while ((0x88 & target) == 0) {
            int targetPiece = bd.board[target];

            if (!isFriendly(targetPiece, bd.m_toMove)) {
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));
                if (targetPiece == EMPTY) {
                    target += offset;
                    continue;
                } else if (isEnemy(targetPiece, bd.m_toMove)) {
                    break;
                }
            }

            if (isFriendly(targetPiece, bd.m_toMove))
                break;
        }
    }
}

void MoveGen::genKingMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq) {

    std::array<int, 8> kingOffsets {-17, -16, -15, -1, 1, 15, 16, 17};


    for (const auto offset : kingOffsets) {

        auto target = sq + offset;

        if ((0x88 & target) == 0) {
            auto targetPiece = bd.board[target];

            if (!isFriendly(targetPiece, bd.m_toMove)) {
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));
                continue;
            }

        }


    }
}

bool MoveGen::isFriendly(int piece, int myColor) {

    // early exit if piece is not on valid sq or sq is empty
    if (piece == EMPTY || piece == INVALID) return false;
    
    if (myColor == WHITE_MOVE) return (piece >= WPAWN && piece <= WKING);
    if (myColor == BLACK_MOVE) return (piece >= BPAWN && piece <= BKING);

    return false; 

}

bool MoveGen::isEnemy(int piece, int myColor) {
    
    if (piece == EMPTY || piece == INVALID) return false;

    if (myColor == WHITE_MOVE) return (piece >= BPAWN && piece <= BKING);
    if (myColor == BLACK_MOVE) return (piece >= WPAWN && piece <= WKING);

    return false;
}


