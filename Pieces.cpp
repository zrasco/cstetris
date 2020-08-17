#include "Pieces.h"
#include <cstring>
#include <cassert>

Piece::Piece(   unsigned int yPos,
                unsigned int xPos,
                unsigned int widthBox,
                unsigned int heightBox)
{
    Piece::xPos = xPos;
    Piece::yPos = yPos;
    Piece::widthBox = widthBox;
    Piece::heightBox = heightBox;
    
    memset(shape,0,sizeof(char) * SHAPE_X * SHAPE_Y);
}

void Piece::Rotate(bool clockwise)
{
    unsigned int temp;
    char newShape[SHAPE_Y][SHAPE_X];
    
    // Zero out the new shape
    memset(&newShape,0,sizeof(char) * SHAPE_X * SHAPE_Y);
    
    if (clockwise)
    {
        for (int x = 0; x < heightBox; x++)
        {
            for (int y = 0; y < widthBox; y++)
            {
                newShape[y][x] = shape[heightBox - 1 - x][y];
            }
        }

    }
    
    //newShape[heightBox - y][x] = shape[heightBox - x][y];
    
    // Swap width and height boxes
    temp = widthBox;
    widthBox = heightBox;
    heightBox = temp;
    
    // Zero out the old shape
    memset(&shape,0,sizeof(char) * SHAPE_X * SHAPE_Y);
    
    // Copy into new shape
    for (int y = 0; y < SHAPE_Y; y++)
    {
        for (int x = 0; x < SHAPE_X; x++)
        {
            shape[y][x] = newShape[y][x];
        }
    } 
}

bool Piece::IsOccupiedInBox(unsigned int relativeY, unsigned int relativeX)
{
    assert(relativeY <= heightBox);
    assert(relativeX <= widthBox);
    
    return shape[relativeY][relativeX] == 'X';
}

char *Piece::operator[](const unsigned int index)
{
    return shape[index];
}

SquarePiece::SquarePiece(unsigned int yPos, unsigned int xPos) : Piece(
                                                                    yPos,
                                                                    xPos,
                                                                    2,
                                                                    2)
{
    shape[0][0] = 'X';
    shape[0][1] = 'X';
    shape[1][0] = 'X';
    shape[1][1] = 'X';
}

TPiece::TPiece(unsigned int yPos, unsigned int xPos) : Piece(
                                                            yPos,
                                                            xPos,
                                                            3,
                                                            2)
{
    
    shape[0][1] = 'X';
    shape[1][0] = 'X';
    shape[1][1] = 'X';
    shape[1][2] = 'X';
}

LongPiece::LongPiece(unsigned int yPos, unsigned int xPos) : Piece(
                                                            yPos,
                                                            xPos,
                                                            1,
                                                            4)
{
    
    shape[0][0] = 'X';
    shape[1][0] = 'X';
    shape[2][0] = 'X';
    shape[3][0] = 'X';
}

LeftZigPiece::LeftZigPiece(unsigned int yPos, unsigned int xPos) : Piece(
                                                                    yPos,
                                                                    xPos,
                                                                    3,
                                                                    2)
{
    
    shape[0][0] = 'X';
    shape[0][1] = 'X';
    shape[1][1] = 'X';
    shape[1][2] = 'X';
}

RightZigPiece::RightZigPiece(unsigned int yPos, unsigned int xPos) : Piece(
                                                                    yPos,
                                                                    xPos,
                                                                    3,
                                                                    2)
{
    
    shape[0][1] = 'X';
    shape[0][2] = 'X';
    shape[1][0] = 'X';
    shape[1][1] = 'X';
}

LeftLPiece::LeftLPiece(unsigned int yPos, unsigned int xPos) : Piece(
                                                                    yPos,
                                                                    xPos,
                                                                    2,
                                                                    3)
{
    shape[0][1] = 'X';
    shape[1][1] = 'X';
    shape[2][0] = 'X';
    shape[2][1] = 'X';
}

RightLPiece::RightLPiece(unsigned int yPos, unsigned int xPos) : Piece(
                                                                    yPos,
                                                                    xPos,
                                                                    2,
                                                                    3)
{
    shape[0][0] = 'X';
    shape[1][0] = 'X';
    shape[2][0] = 'X';
    shape[2][1] = 'X';
}


