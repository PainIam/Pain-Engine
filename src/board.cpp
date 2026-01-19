#include "board.h"
#include "move.h"
#include "movegen.h"



Board::Board() : 
    m_toMove(1), 
    m_BlackCastle(static_cast<int> (castling::NONE)), 
    m_WhiteCastle(static_cast<int> (castling::NONE)),
    m_enPas(NO_ENPAS), 
    m_halfMoves(0),
    m_fullMoves(1)  {
    // create temp variable which should nicely initialize board
    std::array<int, BOARD_SIZE> start = {
    BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID,
    BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID,
    WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID,
    WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID
    };

    // initialize board with start
    board = start;
}




int Board::findKingSquare(int side) {
    int king = (side == WHITE_MOVE) ? WKING : BKING;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((i & 0x88) == 0 && board[i] == king)
            return i;
    }
    return -1;
}


long Board::perft(int depth) {
    if (depth == 0) return 1;

    std::vector<Move> moves;
    // Call your new static function
    MoveGen::moveGen(*this, moves); 

    long nodes = 0;
    for (auto& m : moves) {
        undoInfo u;
        makeMove(m, u);
        nodes += perft(depth - 1);
        unMakeMove(m);
    }
    return nodes;
}



/*
    move function, updates the board and the internal variables like 
    m_enpas, half and full moves clock
*/

void Board::makeMove(const Move& move, undoInfo& undo) {
    
    undo.oldEnpas  = m_enPas;
    undo.oldBlackCastle = m_BlackCastle;
    undo.oldWhiteCatle = m_WhiteCastle;
    undo.capturedPiece = board[move.toSq];
    undo.oldhalfmoves = m_halfMoves;
    history.push_back(undo);

    m_enPas = NO_ENPAS;
    m_toMove *= -1; 


    // counters

    /*half move is incremented if there's no pawn movement or capture*/
    if (move.pieceType != WPAWN && move.pieceType != BPAWN && move.capture == 0) // 0 IS THE VALUE OF AN EMPTY SQ
        m_halfMoves++;
    else m_halfMoves = 0; // reset the counter otherwise

    /*full move is increment only after black moves*/
    if (move.pieceType > WKING)
        m_fullMoves++;

    // what type of move was made
    switch (move.moveType) {
        
        case static_cast<int> (moveType::ORDINARY): {

            board[move.toSq] = move.pieceType;
            board[move.fromSq] = EMPTY;

            // if it's a pawn move and the pawn moved 2 squares, that means there's an en passant available
            if ((std::abs(move.toSq - move.fromSq) == 32) && (move.pieceType == WPAWN || move.pieceType == BPAWN)) {

                m_enPas = move.fromSq + (move.toSq - move.fromSq) / 2;
            }
            break;
        }
        case static_cast<int> (moveType::KING_SIDE): {
            // whites
            if (move.pieceType == WKING) {
                board[117] = WROOK;
                board[118] = WKING;
                board[116] = EMPTY;
                board[119] = EMPTY;
                m_WhiteCastle = static_cast<int> (castling::NONE);
            }
            // blacks
            if (move.pieceType == BKING){
                board[5] = BROOK;
                board[6] = BKING;
                board[4] = EMPTY;
                board[7] = EMPTY;  
                m_BlackCastle = static_cast<int> (castling::NONE);

            }
            break;
        } case static_cast<int>(moveType::QUEEN_SIDE) : {

            if (move.pieceType == WKING) {
                board[116] = EMPTY;
                board[114] = WKING;
                board[112] = EMPTY;
                board[115] = WROOK;
                m_WhiteCastle = static_cast<int> (castling::NONE);
            }
            if (move.pieceType == BKING) {
                board[4] = EMPTY;
                board[0] = EMPTY;
                board[3] = BROOK;
                board[2] = BKING;
                m_BlackCastle = static_cast<int> (castling::NONE);
            }
            break;
        } case static_cast<int> (moveType::ENPASSANT) : {
            board[move.toSq] = move.pieceType;
            board[move.fromSq] = EMPTY;

            // capture the pawn
            if (move.pieceType == WPAWN) {
                board[move.toSq + 16] = EMPTY;
            } else board[move.toSq - 16] = EMPTY;
            break;
        } default : {


            if (m_toMove == BLACK_MOVE) {
                switch (move.moveType) {
                    case static_cast<int> (moveType::PROMO_QUEEN) : board[move.toSq] = WQUEEN; break;
                    case static_cast<int> (moveType::PROMO_ROOK)  : board[move.toSq] = WROOK; break;
                    case static_cast<int> (moveType::PROMO_BISHOP): board[move.toSq] = WBISHOP; break;
                    case static_cast<int> (moveType::PROMO_KNIGHT): board[move.toSq] = WKNIGHT; break;   
                }
            } else {
                switch (move.moveType) {
                    case static_cast<int> (moveType::PROMO_QUEEN) : board[move.toSq] = BQUEEN; break;
                    case static_cast<int> (moveType::PROMO_ROOK)  : board[move.toSq] = BROOK; break;
                    case static_cast<int> (moveType::PROMO_BISHOP): board[move.toSq] = BBISHOP; break;
                    case static_cast<int> (moveType::PROMO_KNIGHT): board[move.toSq] = BKNIGHT; break;   
                }          
            }
            board[move.fromSq] = EMPTY;
        }
    }

    // handle castling
    if (m_BlackCastle != static_cast<int> (castling::NONE) || m_WhiteCastle != static_cast<int> (castling::NONE)) {
        
        // if black king moved, remove all castling rights
        if (board[4] != BKING)
            m_BlackCastle = static_cast<int> (castling::NONE);

        // the converse
        if (board[116] != WKING)
            m_WhiteCastle = static_cast<int> (castling::NONE);

        // remove black long castle
        if (board[0] != BROOK) {
            if (m_BlackCastle == static_cast<int> (castling::BOTH))
                m_BlackCastle = static_cast<int> (castling::SHORT);
            else m_BlackCastle = static_cast<int> (castling::NONE); // only long available so remove it
        }
        
        //remove black short castle
        if (board[7] != BROOK) {
            if (m_BlackCastle == static_cast<int> (castling::BOTH))
                m_BlackCastle = static_cast<int> (castling::LONG);
            else m_BlackCastle = static_cast<int> (castling::NONE);
        }

        // REMOVE WHITE LONG CASTLE
        if (board[112] != WROOK) {
            if (m_WhiteCastle == static_cast<int> (castling::BOTH))
                m_WhiteCastle = static_cast<int> (castling::SHORT);
            else m_WhiteCastle = static_cast<int> (castling::NONE);
        }

        // REMOVE WHITE SHORT CASTLE
        if (board[119] != WROOK) {
            if (m_WhiteCastle == static_cast<int> (castling::BOTH))
                m_WhiteCastle = static_cast<int> (castling::LONG);
            else m_WhiteCastle = static_cast<int> (castling::NONE);
        }
    }

}


void Board::unMakeMove(const Move& move) {

    undoInfo undo = history.back();
    history.pop_back();

    // restore global state
    m_toMove *= -1;
    m_BlackCastle = undo.oldBlackCastle;
    m_WhiteCastle = undo.oldWhiteCatle;
    m_enPas = undo.oldEnpas;
    m_halfMoves = undo.oldhalfmoves;
    
    if (m_toMove == WHITE_MOVE) m_fullMoves--;

    // restore physical state
    switch (move.moveType) {
        case static_cast<int> (moveType::ORDINARY) : {
            board[move.fromSq] = board[move.toSq];
            board[move.toSq] = undo.capturedPiece;
            break;
        }
        case static_cast<int> (moveType::KING_SIDE) : {
            // whites
            if (move.pieceType == WKING) {
                board[116] = WKING;
                board[119] = WROOK;
                board[118] = EMPTY;
                board[117] = EMPTY;
            }

            // blacks
            if (move.pieceType == BKING) {
                board[4] = BKING;
                board[7] = BROOK;
                board[5] = EMPTY;
                board[6] = EMPTY;
            }
        break;
        } case static_cast<int> (moveType::QUEEN_SIDE) : {
            

            if (move.pieceType == WKING) {
                board[116] = WKING;
                board[112] = WROOK;
                board[114] = EMPTY;
                board[115] = EMPTY;
            }

            if (move.pieceType == BKING) {
                board[4] = BKING;
                board[0] = BROOK;
                board[2] = EMPTY;
                board[3] = EMPTY;
            }

            break;
        } case static_cast<int> (moveType::ENPASSANT) : {
            board[move.fromSq] = board[move.toSq];
            board[move.toSq] = EMPTY;

            // resurrect captured piece
            if (move.pieceType == WPAWN)
                board[move.toSq + 16] = BPAWN;
            else
                board[move.toSq - 16] = WPAWN;

            break;
        } default : {
            // promotion
            case static_cast<int> (moveType::PROMO_QUEEN):
            case static_cast<int> (moveType::PROMO_BISHOP):
            case static_cast<int> (moveType::PROMO_KNIGHT):
            case static_cast<int> (moveType::PROMO_ROOK) : {
                board[move.fromSq] = (m_toMove == WHITE_MOVE) ? WPAWN : BPAWN;
                board[move.toSq] = undo.capturedPiece;
            }
            
        }

    }
}



std::string Board::getFen() const {
    std::string fen = "";

    for (int rank = 0; rank < 8; rank++) {
        int empty = 0;
        for (int file = 0; file < 8; file++) {
            int sq = rank * 16 + file;
            int p = board[sq];
            if (p == EMPTY) {
                empty++;
            } else {
                if (empty) { fen += std::to_string(empty); empty = 0; }
                switch(p) {
                    case WKING:   fen += 'K'; break;
                    case WQUEEN:  fen += 'Q'; break;
                    case WROOK:   fen += 'R'; break;
                    case WBISHOP: fen += 'B'; break;
                    case WKNIGHT: fen += 'N'; break;
                    case WPAWN:   fen += 'P'; break;

                    case BKING:   fen += 'k'; break;
                    case BQUEEN:  fen += 'q'; break;
                    case BROOK:   fen += 'r'; break;
                    case BBISHOP: fen += 'b'; break;
                    case BKNIGHT: fen += 'n'; break;
                    case BPAWN:   fen += 'p'; break;
                }
            }
        }
        if (empty) fen += std::to_string(empty);
        if (rank != 7) fen += '/';
    }

    fen += ' ';
    fen += (m_toMove == WHITE_MOVE) ? 'w' : 'b';
    fen += ' ';

    if (m_WhiteCastle == 0 && m_BlackCastle == 0) fen += '-';
    else {
        if (m_WhiteCastle & static_cast<int>(castling::SHORT)) fen += 'K';
        if (m_WhiteCastle & static_cast<int>(castling::LONG))  fen += 'Q';
        if (m_BlackCastle & static_cast<int>(castling::SHORT)) fen += 'k';
        if (m_BlackCastle & static_cast<int>(castling::LONG))  fen += 'q';
    }

    fen += ' ';

    if (m_enPas == NO_ENPAS) fen += '-';
    else {
        int file = m_enPas & 0xF;
        int rank = m_enPas >> 4;
        fen += ('a' + file);
        fen += ('1' + rank);
    }

    fen += ' ';
    fen += std::to_string(m_halfMoves);
    fen += ' ';
    fen += std::to_string(m_fullMoves);

    return fen;
}



/*
    takes an fen string and sets the board as per the fen
*/

void Board::setFen(const std::string& fen) {
    // Clear board
    for (int i = 0; i < BOARD_SIZE; i++) board[i] = EMPTY;

    int rank = 0; // 0 = rank 8
    int file = 0;

    int stage = 0; // 0=pieces,1=toMove,2=castling,3=enPas,4=half/full moves
    size_t i = 0;

    while (i < fen.size()) {
        char c = fen[i];

        if (c == ' ') {
            stage++;
            i++;
            continue;
        }

        switch(stage) {
            case 0: // piece placement
                if (c == '/') {
                    rank++;
                    file = 0;
                } else if (c >= '1' && c <= '8') {
                    file += c - '0';
                } else {
                    int square = rank * 16 + file;
                    switch(c) {
                        case 'K': board[square] = WKING; break;
                        case 'Q': board[square] = WQUEEN; break;
                        case 'R': board[square] = WROOK; break;
                        case 'B': board[square] = WBISHOP; break;
                        case 'N': board[square] = WKNIGHT; break;
                        case 'P': board[square] = WPAWN; break;

                        case 'k': board[square] = BKING; break;
                        case 'q': board[square] = BQUEEN; break;
                        case 'r': board[square] = BROOK; break;
                        case 'b': board[square] = BBISHOP; break;
                        case 'n': board[square] = BKNIGHT; break;
                        case 'p': board[square] = BPAWN; break;
                    }
                    file++;
                }
                break;

            case 1: // turn
                m_toMove = (c == 'w') ? WHITE_MOVE : BLACK_MOVE;
                break;

            case 2: // castling
                m_WhiteCastle = m_BlackCastle = static_cast<int>(castling::NONE);
                if (c != '-') {
                    for (size_t j = i; j < fen.size() && fen[j] != ' '; j++) {
                        switch(fen[j]) {
                            case 'K': m_WhiteCastle |= static_cast<int>(castling::SHORT); break;
                            case 'Q': m_WhiteCastle |= static_cast<int>(castling::LONG); break;
                            case 'k': m_BlackCastle |= static_cast<int>(castling::SHORT); break;
                            case 'q': m_BlackCastle |= static_cast<int>(castling::LONG); break;
                        }
                    }
                }
                break;

            case 3: // en passant
                if (c == '-') m_enPas = NO_ENPAS;
                else {
                    int file_ep = c - 'a';
                    i++;
                    int rank_ep = fen[i] - '1';
                    m_enPas = rank_ep * 16 + file_ep;
                }
                break;

            case 4: // halfmove/fullmove
            {
                size_t j = i;
                // halfmoves
                while (j < fen.size() && fen[j] != ' ') j++;
                m_halfMoves = std::stoi(fen.substr(i, j-i));
                i = j+1;
                // fullmoves
                m_fullMoves = std::stoi(fen.substr(i));
                return; // done
            }
        }
        i++;
    }
}


void Board::print() const {
    std::string pieces {".PRNBQKprnbqk"}; // string pieces to map appropriate piece with enum --see board.h

    std::cout << "\n";
    for (unsigned int rank = 0; rank < 8; rank++) {
        std::cout << 8 - rank << "  "; 
        for (unsigned int file = 0; file < 16; file++) {
            int square = (rank * 16) + file; // square variable

            if ((square & 0x88) == 0)
                std::cout << pieces[board[square]] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "   a b c d e f g h" << "\n";
    

}