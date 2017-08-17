#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>
#include <array>
#include "Typedefs.h"

class Board;
class Move;


/**
 * A class representing a game piece, and handling interactions with it.
 * 
 * @author Mckenna Cisler
 * @version 5.18.2015
 */
class Piece
{
    private:
    	int x;
    	int y;
    	bool isKing = false;
    	
    	/**
     	 * Switches this piece to a king
     	 */
		void setKing() { isKing = true; }
		
		/**
		 * Finds all jumping moves originating from this piece.
		 * Does this recursivly; for each move a new imaginary piece will be generated,
		 * and this function will then be called on that piece to find all possible subsequent moves.
		 * @param board The board to work with - assumed to be flipped to correspond to this piece's color.
		 * @param precedingMove The moves preceding the call to search for moves off this piece - only used

		 * in recursion, should be set to null at first call. (if it's not, it means this piece is imaginary).
		 */
		moves_t getAllPossibleJumps(const Board& board, const move_ptr_t precedingMove) const;
		
    public:
    	const bool isWhite;

		/**
		 * Constructor for objects of class Piece
		 * Initializes position and color.
		 * @param x The x position of this piece.
		 * @param y The y position of this piece.
		 * @param isWhite Used to specify if this piece is black or white.
		 */
		Piece(int x, int y, bool isWhite) : x(x), y(y), isWhite(isWhite) {};
		
		/**
		 * @return Returns a two-part array representing the coordinates of this piece's position.
		 */
		coords_t getCoordinates() const;
		
		/**
		 * @return Returns a string representation of this given piece
		 */
		std::string getString() const;
		
		/**
		 * Switches this peice to be a king if it is at the end of the board.
		 * Should be called after every move.
		 */
		void checkIfShouldBeKing(const Board& board);

		/**
		 * Moves this piece's reference of its position (DOES NOT ACTUALLY MOVE ON BOARD)
		 * @param x The x coordinate of the move
		 * @param y The y coordinate of the move
		 */
		void moveTo(int x, int y) { this->x = x; this->y = y; }
		
		/**
		 * Generates all physically possible moves of the given piece.
		 * (Only actually generates the non-jumping moves - jumps are done recusively in getAllPossibleJumps)
		 * @return Returns a list of all the moves (including recusively found jumps), including each individual one involved in every jump.
		 * @param board The board to work with.
		 */
		moves_t getAllPossibleMoves(const Board& board) const;
};
		
#endif
