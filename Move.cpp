#include "Move.h"

#include "Piece.h"
#include "Board.h"
#include "Typedefs.h"

/**
 * @return Returns a two-part array representing the coordinates of this move's starting position.
 */
coords_t Move::getStartingPosition() const
{
    coords_t position;
    position[0] = x1;
    position[1] = y1;
    return position;
}

/**
 * @return Returns a two-part array representing the coordinates of this move's ending position.
 */
coords_t Move::getEndingPosition() const
{
    coords_t position;
    position[0] = x2;
    position[1] = y2;
    return position;
}
    
/**
 * Finds the pieces jumped in this move.
 * (Get's inbetween jumps using recursion)
 * @return Returns an array of pieces that were jumped.
 * @param board The board to look for the pieces on.
 */
std::vector<Piece*> Move::getJumpedPieces(const Board& board) const
{
	// create expandable list of all pieces
    std::vector<Piece*> pieces(0);
    
    // if this move wasn't a jump, it didn't jump a piece!
    if (isJump)
    {
        // the piece this move is jumping should be between the start and end of this move
        // (the average of those two positions)
        int pieceX = (x1 + x2)/2;
        int pieceY = (y1 + y2)/2;
        
        // add this most recent jump...
        pieces.push_back(board.getValueAt(pieceX, pieceY));
        
        // ...but also go back to get the inbetween ones (if we're not the first move)
        if (precedingMove != nullptr)
        {
            std::vector<Piece*> prevJumped = precedingMove->getJumpedPieces(board);
            pieces.insert(pieces.end(), prevJumped.begin(), prevJumped.end()); 
            
            // something is wrong (a preceding move isn't a jump) if this returns null, so let the error be thrown
        }
    }
    
    // shorten and return
    pieces.shrink_to_fit();
    return pieces;
}
