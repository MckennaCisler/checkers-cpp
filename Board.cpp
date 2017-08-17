#include "Board.h"

#include "Piece.h"
#include "Move.h"
#include "Typedefs.h"

/**
 * Responsible for generating a brand new board
 * Fills the board with pieces in their starting positions.
 * Adds WHITE pieces at the top to start (so white should move first)
 */
Board::Board()
{
	// In this constructor, we dynamically allocate the pieces becasue
	// we want to use pointers here (we want to move the references to a
	// unique piece around the board array, instead of copying full objects
	// around), but that means we must delete that memory later to avoid 
	// memory leaks. (See the deconstructor below)

    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            // add white pieces to the top (in a checkerboard pattern of black spaces - not on white spaces)
            if (y < 3 && isCheckerboardSpace(x, y))
            {
                setValueAt(x, y, new Piece(x, y, true));
            }
            // ... and black pieces to the bottom in the opposite pattern
            else if (y >= SIZE - 3 && isCheckerboardSpace(x, y))
            {
				setValueAt(x, y, new Piece(x, y, false));
            }
            // AND ensure that all non-occupied spaces are null (we don't have 
            // a default constructor for Pieces, so the normal initilizations are weird here)
            else
            {
            	setValueAt(x, y, nullptr);
            }
        }
    }
}

/**
 * Responsible for generating a board based on another board
 */
Board::Board(const Board& board)
{
	for (int pos = 0; pos < SIZE*SIZE; pos++)
    {
		setValueAt(pos, board.getValueAt(pos));  
    }
}

/**
 * Responsible for deconstrucing the board (deleting Pieces) when done.
 */
Board::~Board()
{
	for (int pos = 0; pos < SIZE*SIZE; pos++)
    {
    	delete getValueAt(pos);
		setValueAt(pos, nullptr);  
    }
}
    
/**
 * Using the given move and piece, move the piece on the board and apply it to this board.
 * @param move The Move object to execute on the piece and board.
 * @param piece The Piece object that will be moved.
 */
void Board::applyMoveToBoard(const move_ptr_t move, Piece* piece)
{
    // NOTE: at this point, the starting position of the move (move.getStartingPosition) will not neccesarily
    // be equal to the piece's location, because jumping moves have no understanding of the root move
    // and therefore can only think back one jump. WE ARE PRESUMING that the piece given to this function
    // is the one which the move SHOULD be applied to, but due to this issue we can't test this.
    
    coords_t moveStartingPos = piece->getCoordinates();
    coords_t moveEndingPos = move->getEndingPosition();
    
    // find any pieces we've jumped in the process, and remove them as well
    std::vector<Piece*> jumpedPieces = move->getJumpedPieces(*this);
    if (!jumpedPieces.empty())
    {
        // loop over all jumped pieces and remove them
        for (unsigned int i = 0; i < jumpedPieces.size(); i++)
        {
            if (jumpedPieces[i] != nullptr)
            {
                setValueAt(jumpedPieces[i]->getCoordinates()[0], 
                			jumpedPieces[i]->getCoordinates()[1], nullptr);
            }
        }
    }
        
    // and, move this piece (WE PRESUME that it's this piece) from its old spot (both on board and with the piece itself)
    setValueAt(moveStartingPos[0], moveStartingPos[1], nullptr);
    piece->moveTo(moveEndingPos[0], moveEndingPos[1]);
    
    // do a favor to the piece and check if it should now be a king (it'll change itself)
    piece->checkIfShouldBeKing(*this);
    
    // finally, set the move's destination to the piece we're moving
    setValueAt(moveEndingPos[0], moveEndingPos[1], piece);
}
    
/**
 * Sets the space at this number position to the given Piece object.
 * @param position The number position, zero indexed at top left.
 * @param piece The Piece to put in this space, but can be null to make the space empty
 */
void Board::setValueAt(int position, Piece* piece)
{
    coords_t coords = getCoordsFromPos(position); // convert position to coordinates and use that
    setValueAt(coords[0], coords[1], piece);
}

/**
 * Get's the Piece object at this location, but using a single number,
 * which progresses from 0 at the top left to the square of the size at the bottom right
 * @param position This number, zero indexed at top left
 * @return The Piece here. (may be null).
 */
Piece* Board::getValueAt(int position) const
{
    coords_t coords = getCoordsFromPos(position); // convert position to coordinates and use that
    return getValueAt(coords[0], coords[1]); 
}
    
/**
 * Converts a single position value to x and y coordinates.
 * @param position The single position value, zero indexed at top left.
 * @return A two part int array where [0] is the x coordinate and [1] is the y.
 */
coords_t Board::getCoordsFromPos(int position) const
{
    coords_t coords;
    
    // get and use x and y by finding low and high frequency categories
    coords[0] = position % SIZE; // x is low frequency
    coords[1] = position / SIZE; // y is high frequency
    return coords;
}
    
/**
 * Converts from x and y coordinates to a single position value,
 * which progresses from 0 at the top left to the square of the SIZE minus one at the bottom right
 * @param x The x coordinate
 * @param y The y coordinate
 * @return The single position value.
 */
int Board::getPosFromCoords(int x, int y) const
{
    // sum all row for y, and add low frequency x
    return SIZE*y + x;
}
    
/**
 * @return Returns true if the given position on the board represents a "BLACK" square on the checkboard.
 * (The checkerboard in this case starts with a "white" space in the upper left hand corner
 * @param x The x location of the space
 * @param y The y location of the space
 */
bool Board::isCheckerboardSpace(int x, int y) const
{
    // this is a checkerboard space if x is even in an even row or x is odd in an odd row
    return x % 2 == y % 2;
}
    
/**
 * @return Returns true if the given coordinates are over the edge the board
 * @param x The x coordinate of the position
 * @param y The y coordinate of the position
 */
bool Board::isOverEdge(int x, int y) const
{
    return (x < 0 || x >= SIZE ||
            y < 0 || y >= SIZE);
}
    
/**
 * @return Returns true if the given position is over the edge the board
 * @param position The given 0-indexed position value
 */
bool Board::isOverEdge(int position) const
{
    coords_t coords = getCoordsFromPos(position); // convert position to coordinates and use that
    return isOverEdge(coords[0], coords[1]); 
}
