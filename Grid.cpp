#include "Grid.h"
#include <cstring>              // For memset()
#include <cstdlib>              // For srand(), rand()
#include <ctime>                // For time()
#include <cassert>              // For assert(), of course

Grid::Grid( unsigned int gridWidth,
            unsigned int gridHeight,
            char gridPieceChar)
{
    width = gridWidth;
    height = gridHeight;
    pieceChar = gridPieceChar;
 
    // Initialize RNG seed
    srand(time(nullptr));
    
    // Grid pieces
    FallingPiece = &pieceQueue[0];
    NextPiece = &pieceQueue[1];
    
    // Generate a new falling piece to start. We dont need the return value here
    GetNewNextPiece();
    GetNewFallingPiece();
    
    contents = new char *[height];
    
    // Dynamically allocate each row
    for (int x = 0; x < height; x++)
    {
        contents[x] = new char[width + 1];
        
        memset(contents[x], gridPieceChar, width);
        
        //contents[x][0] = 'X';
        // NULL-terminate the end
        contents[x][width] = 0;
    }
}

Grid::~Grid()
{
    for (int x = 0; x < height; x++)
        delete [] contents[x];
    
    delete [] contents;
}

bool Grid::CanRotate(Piece *candidate, bool clockwise)
{
    // Make a new copy of the rotated piece and test that with the collision checker
    Piece rotateTest = *candidate;
    
    rotateTest.Rotate(clockwise);
    
    return !CheckCollision(&rotateTest, rotateTest.yPos, rotateTest.xPos);
}

bool Grid::CheckCollision(Piece *piece, int newYPos, int newXPos)
{
    bool retval = false;                        // No collision by default
    unsigned int newRightBox = newXPos + piece->widthBox;
    unsigned int newBottomBox = newYPos + piece->heightBox;
    
    // Check if outside the boundary
    if (newRightBox > width  || newXPos < 0)
        retval = true;
    else if (newBottomBox > height)
        retval = true;
    else
    {
        // Check if this hits another pre-existing piece in the grid
        for (int y = 0; y < piece->heightBox && !retval; y++)
        {
            for (int x = 0; x < piece->widthBox && !retval; x++)
            {
                // Check for overlap with other occupied square
                if (piece->IsOccupiedInBox(y, x) &&
                    contents[newYPos + y][newXPos + x] == 'X')
                    retval = true;
            }
        }
    }
        
    return retval;
}

// Absorbs a piece into the board and readies the next piece for the player
void Grid::AbsorbPiece(Piece *piece,
                       int clearedRowNumbers[],
                       int &clearedRowsCount)
{
    clearedRowsCount = 0;
    
    for (int x = 0; x < piece->widthBox; x++)
    {
        for (int y = 0; y < piece->heightBox; y++)
        {
            if (piece->IsOccupiedInBox(y, x))
                contents[y + piece->yPos][x + piece->xPos] = 'X';
        }
    }
    
    // Check for cleared rows. We do it in a very basic way for illustration
    // purposes, but this is a great candidate for optimization.
    // The list of cleared rows will be sorted in descending order
    for (int y = height - 1; y >= 0; y--)
    {
        bool rowClear = true;
        
        for (int x = 0; x < width; x++)
        {
            if (contents[y][x] != 'X')
                rowClear = false;
        }
        
        if (rowClear)
            clearedRowNumbers[clearedRowsCount++] = y;
    }

    GetNewFallingPiece();
}

void Grid::ClearRows(int clearedRowNumbers[], int &clearedRowsCount)
{
    // Note: This function assumes clearedRowNumbers is in descending order
    
    // Done in a basic way and can be optimized
    
    // Sanity check
    assert(clearedRowsCount > 0);
    
    for (int y = height - 1; y >= 0; y--)
    {
        bool isClearRow = false;
        
        for (int x = 0; x < clearedRowsCount && !isClearRow; x++)
        {
            if (clearedRowNumbers[x] == y)
                isClearRow = true;
        }
        
        if (isClearRow)
        {
            // Shift all of the rows above it down by one
            for (int y1 = y; y1 > 0; y1--)
                // TODO: Why doesn't this work?
                //*contents[y1] = *contents[y1 - 1];
                for (int x = 0; x < width; x++)
                    contents[y1][x] = contents[y1 - 1][x];
            
            // Cleared rows are now off by 1. Add 1 to all cleared rows to compensate
            for (int x = 0; x < clearedRowsCount; x++)
                clearedRowNumbers[x]++;
                
            // Try this row again in case the one above it was also clear
            y++;
        }
    }
    
    // TODO: Is this really nessecary?
    // Clear top 4 rows.
    for (int x = 0; x < width; x++)
    {
        contents[0][x] = ' ';
        contents[1][x] = ' ';
        contents[2][x] = ' ';
        contents[3][x] = ' ';
    }    
}

void Grid::FillRows(const int level)
{
    unsigned int heightCounter = height - 1;
    
    // Fill in random blocks based on the game level
    for (int y = 0; y < level; y++)
    {
        // Keep track of # filled in so we never get a full row
        unsigned int count = 0;
        
        for (int x = 0; x < width; x++)
        {
            // We have a 1/3 chance of filling a square with an X
            int rnd = rand() % 3;
            
            if (rnd != 0 && count < width - 2)
            {
                contents[heightCounter][x] = 'X';
                count++;
            }
        }
        heightCounter--;            
    }    
}

Piece *Grid::GetNewFallingPiece()
{
    // Drop in the next piece
    pieceQueue[0] = pieceQueue[1];
    
    // Get new next piece. We don't need the return value
    GetNewNextPiece();
    
    FallingPiece->yPos = 4 - FallingPiece->heightBox + 2;
    
    return FallingPiece;
}

Piece *Grid::GetNewNextPiece()
{
    int pieceType = rand() % 7;
    
    // Set up the next falling piece
    switch (pieceType)
    {
        case 0:
            pieceQueue[1] = sqPieceInstance;
            break;
        case 1:
            pieceQueue[1] = tPieceInstance;
            break;
        case 2:
            pieceQueue[1] = longPieceInstance;
            break;
        case 3:
            pieceQueue[1] = leftLPieceInstance;
            break;
        case 4:
            pieceQueue[1] = rightLPieceInstance;
            break;
        case 5:
            pieceQueue[1] = leftZigPieceInstance;
            break;
        case 6:
            pieceQueue[1] = rightZigPieceInstance;
            break;
    }
    
    NextPiece->xPos = 2;
    NextPiece->yPos = 2;
    
    return NextPiece;
}

char *Grid::operator[](const int index)
{
    return contents[index];
}
