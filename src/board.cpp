#include "board.h"



Board::Board() : 
    m_toMove(1), 
    m_BlackCastle(NO_CASTLE), 
    m_WhiteCastle(NO_CASTLE),
    m_enPas(NO_ENPAS), 
    m_halfMoves(0),
    m_fullMoves(1)  {
    // create temp variable which should nicely initialize board
    int start[BOARD_SIZE] = {
    brook, bknight, bbishop, bqueen, bking, bbishop, bknight, brook, off, off, off, off, off, off, off, off,
    bpawn, bpawn, bpawn, bpawn, bpawn, bpawn, bpawn, bpawn, off, off, off, off, off, off, off, off,
    empty, empty, empty, empty, empty, empty, empty, empty, off, off, off, off, off, off, off, off,
    empty, empty, empty, empty, empty, empty, empty, empty, off, off, off, off, off, off, off, off,
    empty, empty, empty, empty, empty, empty, empty, empty, off, off, off, off, off, off, off, off,
    empty, empty, empty, empty, empty, empty, empty, empty, off, off, off, off, off, off, off, off,
    wpawn, wpawn, wpawn, wpawn, wpawn, wpawn, wpawn, wpawn, off, off, off, off, off, off, off, off,
    wrook, wknight, wbishop, wqueen, wking, wbishop, wknight, wrook, off, off, off, off, off, off, off, off
    };

    // initialize board with start
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = start[i];
    }

    

}

std::string Board::getFen() {
    std::string fen = "";

    int rank_inc = 7;
    int curr_sq = 0;
    int empties = 0; // to empties empty squares
 
    while(curr_sq <= 119) {
        if (!(curr_sq & 0x88)) { // if it is not a valid square
            if (empties != 0) {
                fen += std::to_string(empties); // push_back empties of empty squares if we're at end of rank
                empties = 0; // reset the emptieser
            }
            curr_sq += rank_inc; // jump down to the the next rank
            if (curr_sq != 112) fen += "/";
        } else { // valid board square
            if (board[curr_sq] != empty) { // if there's a piece on the square 
                if (empties != 0) fen += std::to_string(empties); // if there were empties before the piece, record it to the string
                empties = 0; // reset the empties counter
            }
            // record the character which we're at
            switch(board[curr_sq]) {
					case wking  : fen += "K";	break;
					case wqueen : fen += "Q";   break;
					case wrook  : fen += "R";	break;
					case wbishop: fen += "B";   break;
					case wknight: fen += "N";   break;
					case wpawn  : fen += "P";   break;
					case bking  : fen += "k";	break;
					case bqueen : fen += "q";   break;
					case brook  : fen += "r";   break;
					case bbishop: fen += "b";   break;
					case bknight: fen += "n";   break;
					case bpawn  : fen += "p";	break;
                    default     : empties++; // increment the empty squares counter if for some reason this is an empty square
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
    if (m_BlackCastle == NO_CASTLE && m_WhiteCastle == NO_CASTLE) fen += "-";
    else {

        // if value in the m_whitecastle is any of these constants defined in .h

        switch (m_WhiteCastle) {
            case SHORT_CASTLE   : fen += "K"; break; 
            case LONG_CASTLE    : fen += "Q"; break;
            case BOTH_CASTLE    : fen += "KQ"; break; 
        }

        switch (m_BlackCastle) {
            case SHORT_CASTLE   : fen += "k"; break; 
            case LONG_CASTLE    : fen += "q"; break;
            case BOTH_CASTLE    : fen += "kq"; break;     
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

    /*  empty board to prepare it with new board structure,
        from the fen string we're parsing
    */
    for (unsigned int i = 0; i < BOARD_SIZE; i++) {
        board[i] = empty;
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
                    case 'K' : board[bindex] = wking;   bindex++; break;
                    case 'Q' : board[bindex] = wqueen;  bindex++; break;
                    case 'B' : board[bindex] = wbishop; bindex++; break;
                    case 'N' : board[bindex] = wknight; bindex++; break;
                    case 'R' : board[bindex] = wrook;   bindex++; break;
                    case 'P' : board[bindex] = wpawn;   bindex++; break;

                    // blacks
                    case 'k': board[bindex] = bking;    bindex++; break;
                    case 'q': board[bindex] = bqueen;   bindex++; break;
                    case 'r': board[bindex] = brook;    bindex++; break;
                    case 'b': board[bindex] = bbishop;  bindex++; break;
                    case 'n': board[bindex] = bknight;  bindex++; break;
                    case 'p': board[bindex] = bpawn;    bindex++; break;
                
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