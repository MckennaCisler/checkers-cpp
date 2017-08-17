#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "Typedefs.h"

class Piece;
class Move;
	
/**
 * Stores and handles interaction with the game board.
 * 
 * @author Mckenna Cisler
 * @version 5.23.2016
 */
class Board
{
    public:
    	// this MUST be constant in order to allocate the required 2D array
    	// without doing it dynamically (which is just asking for 
    	// segmentation faults and memory leaks)
    	const static int SIZE = 8;

		/**
		 * Responsible for generating a brand new board
		 */
		Board();

		/**
		 * Responsible for generating a board based on another board
		 */
		Board(const Board& board);
		
		/**
		 * Responsible for deconstrucing the board (deleting Pieces) when done.
		 */
		~Board();
   
		/**
		 * Using the given move and piece, move the piece on the board and apply it to this board.
		 * @param move The Move object to execute on the piece and board.
		 * @param piece The Piece object that will be moved.
		 */
		void applyMoveToBoard(const move_ptr_t move, Piece* piece);
    
    	/**
		 * Get's the Piece object at this location. (doesn't error check)
		 * @param x The x position of the Piece
		 * @param y The y position of the Piece
		 * @return The Piece here. (May be null)
		 */
		Piece* getValueAt(int x, int y) const { return this->boardArray[y][x]; }
    
		/**
		 * Get's the Piece object at this location, but using a single number,
		 * which progresses from 0 at the top left to the square of the size at the bottom right
		 * @param position This number, zero indexed at top left
		 * @return The Piece here. (may be null).
		 */
		Piece* getValueAt(int position) const;
    
		/**
		 * Converts from x and y coordinates to a single position value,
		 * which progresses from 0 at the top left to the square of the size minus one at the bottom right
		 * @param x The x coordinate
		 * @param y The y coordinate
		 * @return The single position value.
		 */
		int getPosFromCoords(int x, int y) const;
    
		/**
		 * @return Returns true if the given position on the board represents a "BLACK" square on the checkboard.
		 * (The checkerboard in this case starts with a "white" space in the upper left hand corner
		 * @param x The x location of the space
		 * @param y The y location of the space
		 */
		bool isCheckerboardSpace(int x, int y) const;
		
		/**
		 * @return Returns true if the given coordinates are over the edge the board
		 * @param x The x coordinate of the position
		 * @param y The y coordinate of the position
		 */
		bool isOverEdge(int x, int y) const;
		
		/**
		 * @return Returns true if the given position is over the edge the board
		 * @param position The given 0-indexed position value
		 */
		bool isOverEdge(int position) const;
		
	private:
    	Piece* boardArray[SIZE][SIZE];
	
		/**
		 * Sets the space at these coordinates to the given Piece object.
		 * @param x The x position of the Piece
		 * @param y The y position of the Piece
		 * @param piece The Piece to put in this space, but can be null to make the space empty
		 */
		void setValueAt(int x, int y, Piece* piece) 
		{ this->boardArray[y][x] = piece; }
		
		/**
		 * Sets the space at this number position to the given Piece object.
		 * @param position The number position, zero indexed at top left.
		 * @param piece The Piece to put in this space, but can be null to make the space empty
		 */
		void setValueAt(int position, Piece* piece);
		
		/**
		 * Converts a single position value to x and y coordinates.
		 * @param position The single position value, zero indexed at top left.
		 * @return A two part int array where [0] is the x coordinate and [1] is the y.
		 */
		 coords_t getCoordsFromPos(int position) const;
};

#endif
