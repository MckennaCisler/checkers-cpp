#include "Player.h"
#include "AIPlayer.h"
#include "HumanPlayer.h"
#include "Board.h"
#include "Piece.h"
#include "Move.h"

#include <vector>
#include <iostream>

/**
 * File responsible for determining the gamemode (1- or 2-player), running the game, and handling game exit.
 *
 * @author Mckenna Cisler
 * @version 5.18.2016
 */

bool isPlayer1 = true;
bool endGameNow = false; // an easily accesible "end" variable

/**
 * Clears the terminal screen
 */
void clearScreen()
{
	// see http://stackoverflow.com/a/32008479/3155372
	std::cout << "\033[2J\033[1;1H"; 
}

/**
 * Responsible for quickly ending the game
 */
void triggerEndGame()
{
	endGameNow = true;
}

/**
 * Queries the user to determine the requested gamemode
 * @return Returns true if the user wants two-player mode,
 * else false if they want one-player mode.
 */
bool askIfTwoPlayer()
{
	// keep asking to get a valid response
	while (true)
	{
		using namespace std;
	
	    // display message
	    clearScreen();
	    cout << "*******Welcome to checkers!*******\n" << '\n';
	    cout << "Enter 'exit' to exit at any point (or 0 when moving a piece).\n" << '\n';
	    cout << "We offer two game modes:"<< '\n';
	    cout << "[1] 1 Player Mode (vs Computer) - EXPERIMENTAL" << '\n';
	    cout << "[2] 2 Player Mode"<< '\n';
	    cout << "\nWhich one would you like to play? Enter a number: " << '\n';

	    // ask for string, but only accept "1" or "2"
	    string response;
	    getline(cin, response);
	    if (response == "1")  // TODO: trim whitespace
            return false;
	    else if (response == "2")
            return true;
        else if (response == "exit")
        {
            triggerEndGame();
            return true;
        }
    }
}

/**
 * Determines whether the game has been completed, or is in a stalemate
 * @param board The board to check to determine if we're at an endgame point.
 */
bool endGame(const Board& board)
{
	// have an emergency trigger for endgame
	if (endGameNow)
	    return true;
	else
	{
	    // otherwise search the board for pieces of both colors, and if none of one color are present,
	    // the other player has won.
	    int movableWhiteNum = 0;
	    int movableBlackNum = 0;
	    for (int pos = 0; pos < Board::SIZE*Board::SIZE; pos++)
	    {
	        // make sure the piece exists, and if so sum movable pieces for each color)
	        Piece* pieceHere = board.getValueAt(pos);
	        if (pieceHere != nullptr)
	        {
	            // only consider piece if it has possible moves
	            if (!pieceHere->getAllPossibleMoves(board).empty())
	            {
	                if (pieceHere->isWhite)
	                    movableWhiteNum++;
	                else
	                    movableBlackNum++;
	            }
	        }
	    }
		
		using namespace std;
		
	    // determine if anyone won (or if no one had any moves left)
	    if (movableWhiteNum + movableBlackNum == 0)
	        cout << "The game was a stalemate..." << endl;
	    else if (movableWhiteNum == 0)
	        cout << "Congratulations, Black, you have won the game gloriously!" << endl;
	    else if (movableBlackNum == 0)
	        cout << "Congratulations, White, you have won the game gloriously!" << endl;
	    else
	        return false;

	    // we can only make it here if any of the above conditions are hit
	    return true;
	}
}

int main()
{
	// generate basic board and setup
	Board board;

	// define abstract classes, to be assigned a concrete class after deciding gamemode
	// defined as pointers and dynamically allocated in order to use polymorphism
	Player* player1;
	Player* player2;

	if (askIfTwoPlayer())
	{
	    player1 = new HumanPlayer(true);
	    player2 = new HumanPlayer(false);
	}
	else
	{
	    player1 = new HumanPlayer(true);
	    //player2 = new HumanPlayer(false);
	    player2 = new AIPlayer(false);
	}
	clearScreen();

	while ( !endGame(board) )
	{
	    if (isPlayer1)
	    {
	        player1->getMove(board);
	    }
	    else
	    {
	        player2->getMove(board);
	    }

	    // switch players
	    isPlayer1 = !isPlayer1;
	}
	
	delete player1;
	delete player2;
	
	return 0;
}

