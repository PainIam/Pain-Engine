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
                    // if (piece == WKNIGHT || piece == BKNIGHT)
                    //     MoveGen::genKnightMoves(bd, piece , moves, i);   
                    // if (piece == WROOK || piece == BROOK)
                    //     MoveGen::genRookMoves(bd, piece, moves, i);
                    // if (piece == WBISHOP || piece == BBISHOP)
                    //     MoveGen::genBishopMoves(bd, piece, moves, i);
                    // if (piece == WQUEEN || piece == BQUEEN)
                    //     MoveGen::genQueenMoves(bd, piece, moves, i);
                    // if (piece == WKING || piece == BKING)
                    //     MoveGen::genKingMoves(bd, piece, moves, i);
                    if (piece == WPAWN || piece == BPAWN)
                        MoveGen::genPawnMoves(bd, piece, moves, i);
                        
                }
            
            }
        }
    }
}

void MoveGen::genKnightMoves(const Board& bd, int piece,std::vector<Move>& moves, int sq) {

    std::array<int, 8> knightOffsets = {-33, -31, -18, -14, 14, 18, 31, 33};

    // loop through the offsets
    for (const auto& offset : knightOffsets) {

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

    for (const auto& offset : rookOffsets) {

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

    for (const auto& offset : bishopOffsets) {
        
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

    for (const auto& offset : queenOffsets) {
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

void MoveGen::genPawnMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq) {

    if (bd.m_toMove == WHITE_MOVE)
        MoveGen::genWhitePawnMoves(bd, piece, moves, sq);
    if (bd.m_toMove == BLACK_MOVE)
        MoveGen::genBlackPawnMoves(bd, piece, moves, sq);
}


void MoveGen::genWhitePawnMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq) {


    int singlePush = -16;

    // single push logic

    int target = singlePush + sq;
    
    if ((0x88 & target) == 0) {

        int targetPiece = bd.board[target];

        if (targetPiece == EMPTY) {

            // promotion
            int rank = target >> 4;
            if (rank == 0) {

                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_BISHOP)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_KNIGHT)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_QUEEN)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_ROOK)));
            } else
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));

        }
    }

    // double push 

    int dtarget = (singlePush * 2) + sq;

    if ((0x88 & dtarget) == 0) {

        int targetPiece = bd.board[dtarget];

        if (bd.board[target] == EMPTY && targetPiece == EMPTY) {
            
            // only possible at the first if the pawn is at the ini row

            int rank = sq >> 4;

            if (rank == 6) {
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));
            }
        }
    }

    // diagnal
    std::array<int, 2> diagnalOffsets {-15, -17};

    for (const auto& offset : diagnalOffsets) {

        auto targetD = offset + sq;

        if ((0x88 & targetD) == 0) {

            int targetPiece = bd.board[targetD];

            // normal capture with a promotion
            int rank = targetD >> 4;
            if (targetD == 0 && isEnemy(targetPiece, bd.m_toMove)) {

                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_BISHOP)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_KNIGHT)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_QUEEN)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_ROOK)));
            }

            // normal capture
            if (isEnemy(targetPiece, bd.m_toMove)) {

                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));                
            }


        }
    }
}


void MoveGen::genBlackPawnMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq) {


    int singlePush = 16;

    // single push logic

    int target = singlePush + sq;
    
    if ((0x88 & target) == 0) {

        int targetPiece = bd.board[target];

        if (targetPiece == EMPTY) {

            // promotion
            int rank = target >> 4;
            if (rank == 7) {

                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_BISHOP)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_KNIGHT)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_QUEEN)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_ROOK)));
            } else
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));

        }
    }

    // double push 

    int dtarget = (singlePush * 2) + sq;

    if ((0x88 & dtarget) == 0) {

        int targetPiece = bd.board[dtarget];

        if (bd.board[target] == EMPTY && targetPiece == EMPTY) {
            
            // only possible at the first if the pawn is at the ini row

            int rank = sq >> 4;

            if (rank == 1) {
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));
            }
        }
    }

    // diagnal
    std::array<int, 2> diagnalOffsets {15, 17};

    for (const auto& offset : diagnalOffsets) {

        auto targetD = offset + sq;

        if ((0x88 & targetD) == 0) {

            int targetPiece = bd.board[targetD];

            // normal capture with a promotion
            int rank = targetD >> 4;
            if (targetD == 7 && isEnemy(targetPiece, bd.m_toMove)) {

                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_BISHOP)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_KNIGHT)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_QUEEN)));
                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::PROMO_ROOK)));
            }

            // normal capture
            if (isEnemy(targetPiece, bd.m_toMove)) {

                moves.push_back(Move(piece, sq, target, targetPiece, static_cast<int> (moveType::ORDINARY)));                
            }


        }
    }
}

void MoveGen::genKingMoves(const Board& bd, int piece, std::vector<Move>& moves, int sq) {

    std::array<int, 8> kingOffsets {-17, -16, -15, -1, 1, 15, 16, 17};


    for (const auto& offset : kingOffsets) {

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

bool MoveGen::isSquareAttacked(const Board& bd, int sq, int myColor) {

    int piece = bd.board[sq];

    // can knight attack king

    std::array<int, 8> knightOffsets = {-33, -31, -18, -14, 14, 18, 31, 33};

    // loop through the offsets
    for (const auto& offset : knightOffsets) {

        // ini sq + offset equals our target sq
        auto target = offset + sq;
        if ((0x88 & target) == 0) {

            int targetPiece = bd.board[target];
            
            if (isEnemy(targetPiece, myColor) && targetPiece == BKNIGHT) return true;
            if (isEnemy(targetPiece, myColor) && targetPiece == WKNIGHT) return true;
            
        }
    }

    // sliding pieces
    std::array<int, 8> queenOffsets {-17, -16, -15, -1, 1, 15, 16, 17};
    std::array<int, 4> bishopOffsets {-17, -15, 15, 17};
    std::array<int, 4> rookOffsets {-16, -1, 1, 16};

    
    for (const auto& offset : rookOffsets) {

        auto target = offset + sq;
        while ((0x88 & target) == 0) {

            int targetPiece = bd.board[target];
            
            if (isEnemy(targetPiece, myColor) && piece == WROOK) return true;
            if (isEnemy(targetPiece, myColor) && piece == BROOK) return true;
        }

    }

    for (const auto& offset : bishopOffsets) {

        auto target = offset + sq;
        while((0x88 & target) == 0) {

            int targetPiece = bd.board[target];

            if (isEnemy(targetPiece, myColor) && piece == WBISHOP) return true;
            if (isEnemy(targetPiece, myColor) && piece == BBISHOP) return true;
        }
    }

    for (const auto& offset : queenOffsets) {

        auto target = offset + sq;
        while((0x88 & target) == 0) {

            int targetPiece = bd.board[target];

            if (isEnemy(targetPiece, myColor) && piece == WQUEEN) return true;
            if (isEnemy(targetPiece, myColor) && piece == BQUEEN) return true;
        }
    }

    std::array<int, 8> kingOffsets {-17, -16, -15, -1, 1, 15, 16, 17};

    for (const auto& offset : kingOffsets) {

        auto target = offset + sq;

        if ((0x88 & target) == 0) {

            int targetPiece = bd.board[target];

            if (isEnemy(targetPiece, myColor) && piece == WKING) return true;
            if (isEnemy(targetPiece, myColor) && piece == BKING) return true;
        }
    }


    // pawns

    // white captures

    std::array<int, 2> whitePawnOffsets {-15. -17};

    for (const auto& offset : whitePawnOffsets) {

        auto target = offset + sq;

        if ((0x88 & target) == 0) {

            int targetPiece = bd.board[target];

            if (isEnemy(targetPiece, myColor) && piece == BPAWN) return true;
        }
    }

    std::array<int, 2> blackPawnOffsets {-15. -17};

    for (const auto& offset : blackPawnOffsets) {

        auto target = offset + sq;

        if ((0x88 & target) == 0) {

            int targetPiece = bd.board[target];

            if (isEnemy(targetPiece, myColor) && piece == WPAWN) return true;
        }
    }

    


    return false;
}

