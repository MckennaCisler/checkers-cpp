# the compiler:
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-std=c++11 #-g #-Wall

# the build target executable:
TARGET=checkers

# the desired compile command
COMM=-c

# rules:
$(TARGET): main.o AIPlayer.o Board.o HumanPlayer.o Move.o Piece.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o AIPlayer.o Board.o HumanPlayer.o Move.o Piece.o

main.o: main.cpp AIPlayer.h HumanPlayer.h Board.h 
	$(CC) $(CFLAGS) $(COMM) main.cpp

AIPlayer.o: AIPlayer.h AIPlayer.cpp Player.h Typedefs.h
	$(CC) $(CFLAGS) $(COMM) AIPlayer.cpp
	
Board.o: Board.h Board.cpp Piece.h Move.h Typedefs.h
	$(CC) $(CFLAGS) $(COMM) Board.cpp

HumanPlayer.o: HumanPlayer.h HumanPlayer.cpp Board.h Move.h Piece.h Typedefs.h
	$(CC) $(CFLAGS) $(COMM) HumanPlayer.cpp

Move.o: Move.h Move.cpp Piece.h Board.h Typedefs.h
	$(CC) $(CFLAGS) $(COMM) Move.cpp

Piece.o: Piece.h Piece.cpp Board.h Move.h Typedefs.h
	$(CC) $(CFLAGS) $(COMM) Piece.cpp

clean:
	$(RM) $(TARGET) *.o *.gch
