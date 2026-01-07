#include "board.h"



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

std::vector<Move> Board::generateMoves() {
    std::vector<Move> moves;

    // LOOP THROUGH BOARD
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((0x88 & i) == 0) {
            if (board[i] != INVALID && board[i] != EMPTY)
                if (isMyPiece(board[i])) {
                    // found a friendly piece!!!
                    int piece = board[i];

                    // what sorta piece is it ?
                    // let's try knight first
                    if (piece == WKNIGHT || piece == BKNIGHT) {
                        std::array<int, 8> knightOffsets {-33, -31, -18, -14, 14, 18, 31, 33};

                        // loop through the offsets
                        for (auto offset : knightOffsets) {
                            int target = offset + i;
                            if (((0x88 & target) == 0)) {
                                if (isEnemy(target) || board[target] == EMPTY) {
                                    Move move(piece, i, target, board[target], static_cast<int> (moveType::ORDINARY));
                                    moves.push_back(move);
                                }
                            }
                        }
                    }
                }
        }
    }

    return moves;
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
    m_fullMoves--;

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


bool Board::isMyPiece(int piece) {

    if (m_toMove == WHITE_MOVE) {
        if (piece > EMPTY && piece < BPAWN)
            return true;
        return false;
    }

    if (m_toMove == BLACK_MOVE) {
        if (piece > WKING && piece != INVALID)
            return true;
        return false;
    }

    return false; // contingency
}

bool Board::isEnemy(int piece) {

    if (m_toMove == WHITE_MOVE) {
        if (piece > WKING && piece != INVALID)
            return true;
        return false;
    }

    if (m_toMove == BLACK_MOVE) {
        if (piece > EMPTY && piece < BPAWN)
            return true;
        return false;
    }

    return false;
}

std::string Board::getFen() {
    std::string fen = "";

    int rank_inc = 7;
    int curr_sq = 0;
    int empties = 0; // to empties EMPTY squares
 
    while(curr_sq <= 119) {
        if (!(curr_sq & 0x88)) { // if it is not a valid square
            if (empties != 0) {
                fen += std::to_string(empties); // push_back empties of EMPTY squares if we're at end of rank
                empties = 0; // reset the empties counter
            }
            curr_sq += rank_inc; // jump down to the the next rank
            if (curr_sq != 112) fen += "/";
        } else { // valid board square
            if (board[curr_sq] != EMPTY) { // if there's a piece on the square 
                if (empties != 0) fen += std::to_string(empties); // if there were empties before the piece, record it to the string
                empties = 0; // reset the empties counter
            }
            // record the character which we're at
            switch(board[curr_sq]) {
					case WKING  : fen += "K";	break;
					case WQUEEN : fen += "Q";   break;
					case WROOK  : fen += "R";	break;
					case WBISHOP: fen += "B";   break;
					case WKNIGHT: fen += "N";   break;
					case WPAWN  : fen += "P";   break;
					case BKING  : fen += "k";	break;
					case BQUEEN : fen += "q";   break;
					case BROOK  : fen += "r";   break;
					case BBISHOP: fen += "b";   break;
					case BKNIGHT: fen += "n";   break;
					case BPAWN  : fen += "p";	break;
                    default     : empties++; // increment the EMPTY squares counter if for some reason this is an EMPTY square
            }

        }
        curr_sq++; // proceed to next square.
    }

    // end of pieces processing
    fen += " "; // add space 

    // check if it is w/b turn
    if (m_toMove == WHITE_MOVE) fen += "w";
    else fen += "b";

    // end of turn processing
    fen += " "; // more space

    // castling right's check
    if (m_BlackCastle == static_cast<int> (castling::NONE) && m_WhiteCastle == static_cast<int> (castling::NONE)) fen += "-";
    else {

        // if value in the m_whitecastle is any of these constants defined in .h

        switch (m_WhiteCastle) {
            case static_cast<int> (castling::SHORT)   : fen += "K"; break; 
            case static_cast<int> (castling::LONG)    : fen += "Q"; break;
            case static_cast<int> (castling::BOTH)    : fen += "KQ"; break; 
        }

        switch (m_BlackCastle) {
            case static_cast<int> (castling::SHORT)    : fen += "k"; break; 
            case static_cast<int> (castling::LONG)     : fen += "q"; break;
            case static_cast<int> (castling::BOTH)     : fen += "kq"; break;     
        }
    }

    fen += " "; // add space to prepare for next token

    // prepare for enPas
    if (m_enPas == NO_ENPAS) fen += "-";
    else {

        // there's an enpassent square available

        /*
         find the exact rank and file
         indices in 0x88 are represented by the first4 bits representing the rank, the last the file
         we can isolate last 4 bits by shifting the first 4 like index >> 4, if we have 0000 1111, 
         shifting it gives us 0000

         for files we isolate the first 4 by ANDing the index with 0xF(15), which in binary is 0000 1111
         and thing with any byte will result in the first 4 bits being 0, thus resulting in 1111 assuming
         we AND-ed it with 15
        */

        switch(m_enPas & 0xF) {
            case 0  : fen += "a"; break;
            case 1  : fen += "b"; break;
            case 2  : fen += "c"; break;
            case 3  : fen += "d"; break;
            case 4  : fen += "e"; break;
            case 5  : fen += "f"; break;
            case 6  : fen += "g"; break;
            case 7  : fen += "h"; break;
            default :
                fen += "error in enPas square";
        }

        switch(m_enPas >> 4) {
            case 3  : fen += "3"; break;
            case 6  : fen += "6"; break;
            default : 
                fen += "error in enPas square"; 
        }
    }

    fen += " ";

    // half moves since last capture or pawn movement
    fen += std::to_string(m_halfMoves);

    fen += " ";

    // full moves since game, starts at 1, incremented after first black move
    fen += std::to_string(m_fullMoves);

    //return fen string
    return fen;

}


/*
    takes an fen string and sets the board as per the fen
*/

void Board::setFen(const std::string & fen) {

    /*  EMPTY board to prepare it with new board structure,
        from the fen string we're parsing
    */
    for (unsigned int i = 0; i < BOARD_SIZE; i++) {
        board[i] = EMPTY;
    }

    int i = 0; // keeps track of where we are in the fen str

    int bindex = 0; // where we are in the box, starts with 0, because my a8 = 0

    int currStage = 0; // makes it easier to tell which stage of the fen we're at

    char currChar; // to store current character of the str

    while (i < fen.size()) { // iterate until the end of the fen

        currChar = fen[i];

        // do we need to go to the next stage of the fen ?
        if (currChar == ' ') {
            i++; // go to the next fen character
            currChar = fen[i];
            currStage++;
        }

        // what stage are we at ?
        switch (currStage) {
            case 0 : {
                if (currChar == '/') bindex += 8; // my 0x88 layout allows this
                switch (currChar) {
                    // whites
                    case 'K' : board[bindex] = WKING;   bindex++; break;
                    case 'Q' : board[bindex] = WQUEEN;  bindex++; break;
                    case 'B' : board[bindex] = WBISHOP; bindex++; break;
                    case 'N' : board[bindex] = WKNIGHT; bindex++; break;
                    case 'R' : board[bindex] = WROOK;   bindex++; break;
                    case 'P' : board[bindex] = WPAWN;   bindex++; break;

                    // blacks
                    case 'k': board[bindex] = BKING;    bindex++; break;
                    case 'q': board[bindex] = BQUEEN;   bindex++; break;
                    case 'r': board[bindex] = BROOK;    bindex++; break;
                    case 'b': board[bindex] = BBISHOP;  bindex++; break;
                    case 'n': board[bindex] = BKNIGHT;  bindex++; break;
                    case 'p': board[bindex] = BPAWN;    bindex++; break;
                
                    // the dafault case is that it is a number <= 8, so skip those x squares 
                    default : bindex += currChar - '0'; break;
                }
                break;
            }
            case 1 : {
                // who's turn to move
                if (currChar == 'w') m_toMove = WHITE_MOVE;
                else m_toMove = BLACK_MOVE;
                break;
            }
            case 2 : {
                // castling
                if (currChar == '-') {
                    m_WhiteCastle = static_cast<int>(castling::NONE);
                    m_BlackCastle = static_cast<int>(castling::NONE); // if there's no castling, available
                }

                // whites
                case 'K' : m_WhiteCastle = static_cast<int>(castling::SHORT); break;
                case 'Q' : {
                    if (m_WhiteCastle == static_cast<int>(castling::SHORT))
                        m_WhiteCastle = static_cast<int>(castling::BOTH);
                        break;
                }

                // blacks
                case 'k' : m_BlackCastle = static_cast<int>(castling::SHORT); break;
                case 'q' : {
                    if (m_BlackCastle == static_cast<int>(castling::SHORT))
                        m_BlackCastle = static_cast<int>(castling::BOTH);
                        break;
                }


                break;
            } 
            case 3 : {
                // enpas
                if (currChar == '-') m_enPas = NO_ENPAS;

                // FIND THE FILE
                int file = currChar - 'a';

                // FIND THE RANK

                i++; // goto next char
                currChar = fen[i];
                if (currChar == '3' || currChar == '6') {
                    currChar = fen[i];
                    int rank = (currChar - '0') - 1;
                    m_enPas = (rank * 16) + file;
                } else m_enPas = NO_ENPAS; // invalid enpas square check
            }
            case 4 : {
                if (fen[i+1] == ' ') {
                    // the half move is a single digit character
                    m_halfMoves = currChar - '0';
                    i++; // time to move to full moves
                } else {
                    std::string temp = "";
                    temp += currChar;
                    temp += fen[i+1]; // store a 2 digit string and later convert to int
                    m_halfMoves = std::stoi(temp);
                    i+=2; // go to full moves
                }
                i++;
                std::string tmp = "";
                for(; i < fen.size(); i++)
                    tmp += fen[i];
                m_fullMoves = std::stoi(tmp);

                break;
            }

        }
        i++; // go to next char
    }

}


void Board::print() const {
    std::string pieces {".PRNBQKprnbqk"}; // string pieces to map appropriate piece with enum --see board.h

    std::cout << endl;
    for (unsigned int rank = 0; rank < 8; rank++) {
        std::cout << 8 - rank << "  "; 
        for (unsigned int file = 0; file < 16; file++) {
            int square = (rank * 16) + file; // square variable

            if ((square & 0x88) == 0)
                std::cout << pieces[board[square]] << " ";
        }
        std::cout << endl;
    }
    std::cout << "   a b c d e f g h" << endl;
    

}