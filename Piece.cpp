#include "Piece.h"

#include "Board.h"
#include "Move.h"
#include "Typedefs.h"

#include <iostream>

/**
 * @return Returns a two-part array representing the coordinates of this piece's position.
 */
coords_t Piece::getCoordinates() const
{
    coords_t coords;
    coords[0] = this->x;
    coords[1] = this->y;
    return coords;
}
    
/**
 * @return Returns a string representation of this given piece
 */
std::string Piece::getString() const
{
    std::string baseSymbol;

    if (isWhite)
        baseSymbol = "W";
    else
        baseSymbol = "B";

    if (isKing)
        baseSymbol += "K";
    else
        baseSymbol += " "; // add a space in the non-king state just to keep consistency

    return baseSymbol;
}

    
/**
 * Switches this peice to be a king if it is at the end of the board.
 * Should be called after every move.
 */
void Piece::checkIfShouldBeKing(const Board& board)
{
    // if the piece is white, it's a king if it's at the +y, otherwise if its black this happens at the -y side
    if (isWhite && this->y == Board::SIZE - 1 || 
        !isWhite && this->y == 0)
        setKing();
}
    
/**
 * Generates all physically possible moves of the given piece.
 * (Only actually generates the non-jumping moves - jumps are done recusively in getAllPossibleJumps)
 * @return Returns a list of all the moves (including recusively found jumps), including each individual one involved in every jump.
 * @param board The board to work with - assumed to be flipped to correspond to this piece's color.
 */
moves_t Piece::getAllPossibleMoves(const Board& board) const
{
    // create expandable list of all moves
    moves_t moves(0);
    
    // change y endpoints based on kingness and color=direction of movement
    int startingY, yIncrement;
    if (isWhite)
    {
        // if it's white, we move from further down the board backwards to possible king position
        startingY = this->y + 1; 
        yIncrement = -2;
    }
    else 
    {
        // if it's black, we move from further up the board forward to possible king position
        startingY = this->y - 1;
        yIncrement = 2;
    }
    
    // use kingess to determine number of rows to check
    int rowsToCheck = 1; // default as non-king
    if (this->isKing)
        rowsToCheck = 2;
    
    // iterate over the four spaces where normal (non-jumping) moves are possible        
    for (int x = this->x - 1; x <= this->x + 1; x += 2)
    {
        // go over the rows (or row) (we iterate the number of times determined by the kingess above)
        int y = startingY - yIncrement; // add this so we can add the normal increment before the boundary checks
        for (int i = 0; i < rowsToCheck; i++) 
        {
            // increment y if we need to (this will have no effect if we only run one iteration)
            y += yIncrement;
            
            // check for going off end of board, in which case just skip this iteration (we may do this twice if at a corner)
            if (board.isOverEdge(x, y))
                continue;
            
            // add a move here if there's not a piece 
            if (board.getValueAt(x, y) == nullptr)
            {
                // this is not jump move in any case, and is always the first move
                move_ptr_t move(new Move(this->x, this->y, x, y, nullptr, false));
                moves.push_back(move); 
            }
        }
    }
    
    // after we've checked all normal moves, look for and add all possible jumps (recusively as well - I mean ALL jumps)
    moves_t possibleJumps = this->getAllPossibleJumps(board, nullptr);
    moves.insert(moves.end(), possibleJumps.begin(), possibleJumps.end());

    moves.shrink_to_fit();
    return moves;
}
    
/**
 * Finds all jumping moves originating from this piece.
 * Does this recursivly; for each move a new imaginary piece will be generated,
 * and this function will then be called on that piece to find all possible subsequent moves.
 * @param board The board to work with - assumed to be flipped to correspond to this piece's color.
 * @param precedingMove The moves preceding the call to search for moves off this piece - only used
 * in recursion, should be set to null at first call. (if it's not, it means this piece is imaginary).
 */
moves_t Piece::getAllPossibleJumps(const Board& board, const move_ptr_t precedingMove) const
{
    // create expandable list of all moves
    moves_t moves(0);
    
    // this is the same as above except we're doing a large cube (4x4)
    // change y endpoints based on kingness and color=direction of movement
    int startingY, yIncrement;
    if (isWhite)
    {
        // if it's white, we move from further down the board backwards to possible king position
        startingY = this->y + 2;
        yIncrement = -4;
    }
    else 
    {
        // if it's black, we move from further up the board forward to possible king position
        startingY = this->y - 2;
        yIncrement = 4;
    }
    
    // use kingess to determine number of rows to check
    int rowsToCheck = 1; // default as non-king
    if (this->isKing)
        rowsToCheck = 2;
    
    // iterate over the four spaces where normal (non-jumping) moves are possible        
    for (int x = this->x - 2; x <= this->x + 2; x += 4)
    {
        // go over the rows (or row) (we iterate the number of times determined by the kingess above)
        int y = startingY - yIncrement; // add this so we can add the normal increment before the boundary checks in the loop
        for (int i = 0; i < rowsToCheck; i++) 
        {
            // increment y if we need to (this will have no effect if we only run one iteration)
            y += yIncrement;
            
            // check for going off end of board, in which case just skip this iteration (we may do this twice if at a corner)
            if (board.isOverEdge(x, y))
                continue;
            
            // don't try to go backward to our old move start so we don't get in infinite recursion loops
            if (precedingMove != nullptr &&
                x == precedingMove->getStartingPosition()[0] && 
                y == precedingMove->getStartingPosition()[1])
                continue;
            
            // test if there is a different-colored piece between us (at the average of our position) and the starting point 
            // AND that there's no piece in the planned landing space (meaning we can possible jump there)
            Piece* betweenPiece = board.getValueAt( (this->x + x)/2 , (this->y + y)/2 );
            if (betweenPiece != nullptr &&
                betweenPiece->isWhite != this->isWhite &&
                board.getValueAt(x, y) == nullptr)
            {
                // in which case, add a move here, and note that it is a jump (we may be following some other jumps)
                move_ptr_t jumpingMove(new Move(this->x, this->y, x, y, precedingMove, true)); // origin points are absolute origin (ORIGINAL piece)
                
                // then add it to our list
                moves.push_back(jumpingMove);
                  
                // after jumping, create an imaginary piece as if it was there to look for more jumps
                Piece imaginaryPiece(x, y, this->isWhite);
                
                // correspond possible jumps to this piece's kingness
                if (this->isKing) imaginaryPiece.setKing();
                
                // find possible subsequent moves recusivly
                moves_t subsequentMoves = imaginaryPiece.getAllPossibleJumps(board, jumpingMove);

                // add these moves to our list if they exist, otherwise just move on to other possibilities
		        moves.insert(moves.end(), subsequentMoves.begin(), subsequentMoves.end());
            }
        }
    }
    moves.shrink_to_fit();
    return moves;
}
