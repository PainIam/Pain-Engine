#include "board.h"



Board::Board() : m_toMove(1), m_BlackCastle(NO_CASTLE), m_WhiteCastle(NO_CASTLE), m_enPas(NO_ENPAS) {
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
        if ((curr_sq & 0x88) != 0) {
            if (empties != 0) {
                fen += std::to_string(empties); // pb empties of empty squares if we're at end of rank
                empties = 0; // reset the emptieser
            }
            curr_sq += rank_inc; // jump down to the the next rank
            if (curr_sq != 112) fen += "/";
        } else { // valid board square
            if (board[curr_sq] != empty) { // we enemptiesered a piece 
                if (empties != 0) fen += std::to_string(empties);
                empties = 0; // reset the empties emptieser
            }
            // if not let's get the character at the board
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
                    default     : empties++; // increment the empty square emptieser if 
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
            case SHORT_CASTLE   : fen += "K"; break; 
            case LONG_CASTLE    : fen += "Q"; break;
            case BOTH_CASTLE    : fen += "KQ"; break;     
        }
    }

    fen += " "; // add space to prepare for next token

    // prepare for enPas
    if (m_enPas == NO_ENPAS) fen += "-";
    else {
        // there's an enpassent square available
    }

}

void Board::print() const {
    std::string pieces {".PRNBQKprnbqk"}; // string pieces to map appropriate piece with enum --see board.h

    std::cout << endl;
    for (int rank = 0; rank < 8; rank++) {
        std::cout << 8 - rank << "  "; 
        for (int file = 0; file < 16; file++) {
            int square = (rank * 16) + file; // square variable

            if ((square & 0x88) == 0)
                std::cout << pieces[board[square]] << " ";
        }
        std::cout << endl;
    }
    std::cout << "   a b c d e f g h" << endl;

}