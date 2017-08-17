#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "Player.h"
#include "Typedefs.h"

class Board;
class Move;
class Piece;

#include <vector>
#include <string>

/**
 * Responsible for communicating with the human player and serving as an interface with the main game engine.
 * 
 * @author Mckenna Cisler
 * @version 5.18.2015
 */
class HumanPlayer : public Player
{
    private:
	    const bool isWhite;
		
		/**
		 * Responsible for displaying the game board to the user (WITH possible moves)
		 * @param board The board to be displayed
		 * @param possibleMoves A vector of possible moves to display while printing the board.
		 */
		void displayBoard(const Board& board, const moves_t possibleMoves);
		
		/**
		 * Responsible for displaying the game board to the user (WITHOUT possible moves)
		 * @param board The board to be displayed
		 */
		void displayBoard(const Board& board);
		
		/**
		 * Asks the user for a piece on the board (for them to move),
		 * and ensures it is an actual piece of the correct color
		 * @param board The board to check against
		 * @return The Piece object to be returned (will be an actual piece)
		 */
		Piece* getPieceFromUser(const Board& board);
		
		/**
		 * Asks the user for a number representing a move of a particular piece,
		 * checking that it is an available move. (The user should be shown all moves beforehand)
		 * @param possibleMoves The list of possible moves the user can request
		 * @return The Move object representing the chosen move (may be null if the user chooses to get a new piece)
		 */
		move_ptr_t getMoveFromUser(const moves_t possibleMoves);
		
		/**
		 * @return Returns a titlecase string representing this player's color
		 */
		std::string getColor();
	    
	public:
		/**
		 * Constructor for the HumanPlayer
		 * @param isWhite Used to specify if this player is black or white.
		 */
		HumanPlayer(bool isWhite) : isWhite(isWhite) {};
		
		/**
		 * Gets a move, by asking the human player what move they want to do.
		 * @param board The board to apply the move to (assumed to be oriented so that this player is on the top)
		 */
		virtual void getMove(Board& board);
};

#endif
