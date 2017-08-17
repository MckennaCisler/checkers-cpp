#ifndef PLY_H
#define PLY_H

class Board;

/**
 * An abstract version of a player, from which Human and AI Players will be extended.
 * Used so that both player types can be used interchangably.
 * 
 * @author Mckenna Cisler
 * @version 5.18.2016
 */
class Player
{
	public:
		/**
		 * Gets a move, by asking the given player what move they want to do.
		 * @param board The board to apply the move to
		 */
		virtual void getMove(Board& board) = 0;
};

#endif
