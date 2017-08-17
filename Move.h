#ifndef MOVE_H
#define MOVE_H

#include <array>
#include <vector>
#include "Typedefs.h"

class Piece;
class Board;

/**
 * Represents a single move of a piece.
 * 
 * @author Mckenna Cisler
 * @version 5.18.2015
 */
class Move
{
	private:
	    const int x1, y1, x2, y2;
    	const move_ptr_t precedingMove;
    	const bool isJump;
    
    public:    
		/**
		 * Constructor for objects of class Move - initializes starting and final position.
		 * @param x1 Starting x position.
		 * @param y1 Starting y position.
		 * @param x2 Ending x position.
		 * @param y2 Ending y position.
		 * @param precedingMove The move preceding this one (can be null if move is first)
		 */
		Move(int x1, int y1, int x2, int y2, const move_ptr_t precedingMove, bool isJump) :
			x1(x1), y1(y1), x2(x2), y2(y2), precedingMove(precedingMove), isJump(isJump)
			{};

		/**
		 * @return Returns a two-part array representing the coordinates of this move's starting position.
		 */
		coords_t getStartingPosition() const;
		
		/**
		 * @return Returns a two-part array representing the coordinates of this move's ending position.
		 */
		coords_t getEndingPosition() const;
		
		/**
		 * Finds the pieces jumped in this move.
		 * (Get's inbetween jumps using recursion)
		 * @return Returns an array of pieces that were jumped.
		 * @param board The board to look for the pieces on.
		 */
		std::vector<Piece*> getJumpedPieces(const Board& board) const;
};

#endif
