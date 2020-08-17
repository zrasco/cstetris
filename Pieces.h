#ifndef PIECES_H
#define PIECES_H

class Piece
{
public:
    Piece(unsigned int = 0, unsigned int = 0, unsigned int = 0, unsigned int = 0);
    void Rotate(bool);
    
    // Operator overloads
    char *operator[](const unsigned int);
    
    // Public functions
    bool IsOccupiedInBox(unsigned int, unsigned int);
    
    // Public members
    unsigned int xPos;                  // Top-left corner
    unsigned int yPos;                  // of the piece
    unsigned int widthBox;
    unsigned int heightBox;

protected:
    // Maximum sizes of shapes
    static const unsigned int SHAPE_Y = 4;
    static const unsigned int SHAPE_X = 4;
    
    char shape[SHAPE_Y][SHAPE_X];
};

class SquarePiece : public Piece
{
public:
    SquarePiece(unsigned int = 0, unsigned int = 0);
};

class TPiece : public Piece
{
public:
    TPiece(unsigned int = 0, unsigned int = 0);
};

class LongPiece : public Piece
{
public:
    LongPiece(unsigned int = 0, unsigned int = 0);
};

class LeftLPiece : public Piece
{
public:
    LeftLPiece(unsigned int = 0, unsigned int = 0);    
};

class RightLPiece : public Piece
{
public:
    RightLPiece(unsigned int = 0, unsigned int = 0);    
};

class LeftZigPiece : public Piece
{
public:
    LeftZigPiece(unsigned int = 0, unsigned int = 0);
};

class RightZigPiece : public Piece
{
public:
    RightZigPiece(unsigned int = 0, unsigned int = 0);
};

#endif      // PIECES_H
