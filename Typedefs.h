#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <vector>
#include <array>
#include <memory>

class Move;

// There are several functions which use two-part arrays to talk 
// about coordinates, so this is defined to make that simpler 
// and more specific (this is a good example of typedefs in general)
// Arrays were much more difficult here than in Java, because
// they just decay to pointers.
typedef std::array<int, 2> coords_t;

// We use shared pointers with the Move arrays and with basic Moves
// because these are "thrown around a lot" between functions,
// so it is nice to not have to keep track of deleting the memory, 
// but we can still use them as pointers and not worry about them being
// copied (this is also important because the hash tables used
// in AIPlayer.cpp basically need moves to be represented as pointers.)
typedef std::vector<std::shared_ptr<Move>> moves_t;
typedef std::shared_ptr<Move> move_ptr_t;

#endif
