#ifndef GRID_H
#define GRID_H

#include "Pieces.h"

class Grid {
public:
    // Constructor
    Grid(unsigned int, unsigned int, char = ' ');
    ~Grid();
    
    // Functions
    bool CanRotate(Piece*, bool);
    bool CheckCollision(Piece*, int, int);
    Piece *GetNewFallingPiece();
    void AbsorbPiece(Piece*, int[], int&);
    void ClearRows(int[], int&);
    void FillRows(const int);
    
    // Operator overloads
    char *operator[](const int);
    
    // Pieces on the grid
    Piece *FallingPiece;
    Piece *NextPiece;
    
    // Piece display chars
    
private:
    unsigned int width;
    unsigned int height;
    char pieceChar;
    
    Piece *GetNewNextPiece();
    
    // Piece queue (not implemented, just use 1)
    static const unsigned int PIECES_IN_QUEUE = 2;
    Piece pieceQueue[PIECES_IN_QUEUE];
    
    // Default piece instances. Need one for each piece
    SquarePiece sqPieceInstance;
    TPiece tPieceInstance;
    LongPiece longPieceInstance;
    LeftLPiece leftLPieceInstance;
    RightLPiece rightLPieceInstance;
    LeftZigPiece leftZigPieceInstance;
    RightZigPiece rightZigPieceInstance;
    
    // Needed for a 2D array
    char **contents;
};
#endif // GRID_H
