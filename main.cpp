#include <chrono>                       // For sleep-related functions
#include <thread>
#include <sys/time.h>                   // For timers
#include <cstdlib>                      // For srand()
#include <cassert>                      // For assert()

#include "Console.h"
#include "Gamestate.h"
#include "WindowWrapper.h"
#include "Grid.h"

using namespace std;

// Function prototypes
double get_time_ms();
void DrawPiece(WindowWrapper*, Piece *, bool = false);

// Game-related
const unsigned int FPS = 60;
const unsigned int FPS_DELAY = 1000 / FPS;

// Window placement
const unsigned short BASE_X = 0;
const unsigned short BASE_Y = 0;

// Width and height of menu
const unsigned int MENU_WIDTH = 30;
const unsigned int MENU_HEIGHT = 20;

// Width and height of board
const unsigned int BOARD_WIDTH = 10;
const unsigned int BOARD_HEIGHT = 24;
const unsigned int BOARD_HEIGHT_VISIBLE = BOARD_HEIGHT - 4;

// Position of the score window relative to the upper-right corner
// of the grid window
const unsigned int SCORE_SPACER_X = 1;
const unsigned int SCORE_SPACER_Y = 0;

// Width and height of next piece window
const unsigned int SCORE_WIDTH = 8;
const unsigned int SCORE_HEIGHT = 6;

// Position of the next piece window relative to the upper-right corner
// of the grid window
const unsigned int NEXTPIECE_SPACER_X = SCORE_SPACER_X + 1;
const unsigned int NEXTPIECE_SPACER_Y = SCORE_HEIGHT + 2;

// Width and height of next piece window
const unsigned int NEXTPIECE_WIDTH = 5;
const unsigned int NEXTPIECE_HEIGHT = 6;

const unsigned int BOX_OFFSET = 2;            // 2x thickness of all box borders

// Piece drop delay
const unsigned int PIECE_DROP_DELAY_MS = 1000;

// Animation phases
const unsigned int ANIMATION_FRAMES_PER_STEP = 5;
const unsigned int ANIMATION_ROWCLEAR_NOTHING = 0;
const unsigned int ANIMATION_ROWCLEAR_DASHES = 1;
const unsigned int ANIMATION_ROWCLEAR_HIGHLIGHT = 2;

// Scores for lines from NES version
const unsigned int LINE_SCORES[] = { 40, 100, 300, 1200 };

// Global game state, console (I/O) and grid abstraction
Gamestate gameState;
Console gameConsole(MENU_WIDTH + BOX_OFFSET,
                    MENU_HEIGHT + BOX_OFFSET);
Grid gameGrid(BOARD_WIDTH, BOARD_HEIGHT);     // Where the action happens
                        
int main()
{
    bool exitFlag = false;
    bool gameOver = false;
    uint totalFrameCounter = 0;
    uint animationFrameCounter = 0;
    const char *lastPressed = "<none>";
    unsigned short menuYValues[] = { 2, 4, 6 };             // Menu items
    unsigned char selectedItem = 1;                         // Selected item index
    double pieceDropTimer = 0.0;
    unsigned int animationStep = ANIMATION_ROWCLEAR_NOTHING;
    int clearedRows[4];                                     // Holds # of cleared rows
    int clearedRowsCount = 0;
    double dropDelay;
    
    gameConsole.LastKey = 0;
    
    // Initialize the 3 game windows
    gameConsole.MenuWindowPtr = new WindowWrapper(  BASE_X,
                                                    BASE_Y,
                                                    MENU_WIDTH + BOX_OFFSET,
                                                    MENU_HEIGHT + BOX_OFFSET);
    gameConsole.GridWindowPtr = new WindowWrapper(  BASE_X,
                                                    BASE_Y,
                                                    BOARD_WIDTH + BOX_OFFSET,
                                                    BOARD_HEIGHT_VISIBLE + BOX_OFFSET);                                                    
    gameConsole.NextPieceWindowPtr = new WindowWrapper( BASE_X + BOARD_WIDTH + BOX_OFFSET + NEXTPIECE_SPACER_X,
                                                        BASE_Y + NEXTPIECE_SPACER_Y,
                                                        NEXTPIECE_WIDTH + BOX_OFFSET,
                                                        NEXTPIECE_HEIGHT + BOX_OFFSET);
                                                        
    gameConsole.ScoreWindowPtr = new WindowWrapper( BASE_X + BOARD_WIDTH + BOX_OFFSET + SCORE_SPACER_X,
                                                        BASE_Y + SCORE_SPACER_Y,
                                                        SCORE_WIDTH + BOX_OFFSET,
                                                        SCORE_HEIGHT + BOX_OFFSET);
                                                                                                                
    // Only the menu is visible at first
    gameConsole.MenuWindowPtr->SetVisibility(true);

    while (!exitFlag)
    {
        int inputCode = 0;
        
        // Enforce FPS
        this_thread::sleep_for(chrono::milliseconds(FPS_DELAY));
        
        // Clear the screen
        //gameConsole.Clear();
        
        // Get the keypress from the user
        unsigned char keyPress = gameConsole.GetKey();
        
        if (keyPress == Console::PRESSED_ENTER && 
            gameConsole.MenuWindowPtr->IsVisible())
        {
            // Used pressed enter, begin game.
            gameConsole.MenuWindowPtr->SetVisibility(false);
            gameConsole.GridWindowPtr->SetVisibility(true);
            gameConsole.NextPieceWindowPtr->SetVisibility(true);
            gameConsole.ScoreWindowPtr->SetVisibility(true);
            
            // Need to clear the screen before we begin
            gameConsole.ClearScreen();
            
            // Initialize the piece drop timer
            dropDelay = PIECE_DROP_DELAY_MS /(gameState.GetSpeed() + 1);
            pieceDropTimer = get_time_ms() + dropDelay;
            
            // Finally, set the grid level
            gameGrid.FillRows(gameState.GetLevel());
            
            // After the work above is done the next rendering cycle will
            // draw the board
        }
        else if (gameConsole.MenuWindowPtr->IsVisible())
        {
            WindowWrapper *mwp = gameConsole.MenuWindowPtr;     // Just an alias
            
            // The various menu manipulation keypress checks
            if (keyPress == Console::PRESSED_UP && selectedItem > 1)
                selectedItem--;
            else if (keyPress == Console::PRESSED_DOWN && selectedItem < 2)
                selectedItem++;
            else if (keyPress == Console::PRESSED_RIGHT && selectedItem == 1)
                gameState.SetSpeed(gameState.GetSpeed() + 1);
            else if (keyPress == Console::PRESSED_LEFT && selectedItem == 1)
                gameState.SetSpeed(gameState.GetSpeed() - 1);
            else if (keyPress == Console::PRESSED_RIGHT && selectedItem == 2)
                gameState.SetLevel(gameState.GetLevel() + 1);
            else if (keyPress == Console::PRESSED_LEFT && selectedItem == 2)
                gameState.SetLevel(gameState.GetLevel() - 1);                
                
            // Put the cursor at the beginning of the window's base coordinates
            mwp->SetCursor(0,0);
            
            // Output each menu item and set the highlighted one accordingly
            for (int y = 0; y < MENU_HEIGHT; y++)
            {
                if (y == menuYValues[selectedItem])
                    mwp->SetReverseText();
                    
                if (y == menuYValues[0])
                    mwp->PrintRow("**** TETRIS ****", true);
                else if (y == menuYValues[1])
                    mwp->PrintRow("Set speed (0-20): " + 
                                    to_string(gameState.GetSpeed()), true);
                else if (y == menuYValues[2])
                    mwp->PrintRow("Set level (0-10): " +
                                    to_string(gameState.GetLevel()), true);
                else
                    *mwp << endl;
                    
                if (y == menuYValues[selectedItem])
                    mwp->ClearReverseText();
            }
            
            mwp->PrintRow("(ENTER to play, ESC to exit)", true);
            
            // Draw the border last to overwrite any weird stuff!
            mwp->Box();
            mwp->Refresh();
        }
        
        // Draw main game grid window & handle all animations in it
        if (gameConsole.GridWindowPtr->IsVisible())
        {
            WindowWrapper *gwp = gameConsole.GridWindowPtr;     // Just an alias
            double timeInMs = get_time_ms();                    // Piece drop timer
            
            // Put the cursor at the beginning of the window's base coordinates
            gwp->SetCursor(1,0);            
            
            if (animationStep == ANIMATION_ROWCLEAR_NOTHING && !gameOver)
            {
                // Check for arrow keypresses. We will rotate/move the piece
                // only after doing a collision check first, otherwise we
                // ignore it.
                if (keyPress == Console::PRESSED_LEFT &&
                    !gameGrid.CheckCollision(gameGrid.FallingPiece,
                                            gameGrid.FallingPiece->yPos,
                                            gameGrid.FallingPiece->xPos - 1))
                    gameGrid.FallingPiece->xPos--;
                else if (keyPress == Console::PRESSED_RIGHT &&
                    !gameGrid.CheckCollision(gameGrid.FallingPiece,
                                            gameGrid.FallingPiece->yPos,
                                            gameGrid.FallingPiece->xPos + 1))
                    gameGrid.FallingPiece->xPos++;
                else if (keyPress == Console::PRESSED_DOWN &&
                    !gameGrid.CheckCollision(gameGrid.FallingPiece,
                                            gameGrid.FallingPiece->yPos + 1,
                                            gameGrid.FallingPiece->xPos))
                    gameGrid.FallingPiece->yPos++;
                else if (keyPress == Console::PRESSED_UP)
                {
                    if (gameGrid.CanRotate(gameGrid.FallingPiece, true))
                        gameGrid.FallingPiece->Rotate(true);
                }                
            }
            
            // Print the X's for the game grid
            for (int x = 4; x < BOARD_HEIGHT; x++)
                *gwp << " " << gameGrid[x] << endl;
            
            // Draw the falling piece or row clear animations
            if (gameOver)
            {
                gwp->SetBoldText();
                
                gwp->SetCursor(10, 1);
                *gwp << "GAME OVER!" << endl;
                *gwp << " (ESC=Exit)" << endl;
                
                gwp->ClearBoldText();
            }
                
            if (animationStep == ANIMATION_ROWCLEAR_NOTHING)
                DrawPiece(gwp, gameGrid.FallingPiece);
            else if (animationStep == ANIMATION_ROWCLEAR_DASHES)
            {
                for (int y = 0; y < clearedRowsCount; y++)
                {
                    gwp->SetCursor(clearedRows[y] - 4 + 1, 1);
                    
                    for (int x = 0; x < BOARD_WIDTH; x++)
                        *gwp << '-';
                }
            }
            else if (animationStep == ANIMATION_ROWCLEAR_HIGHLIGHT)
            {
                gwp->SetReverseText();
                
                for (int y = 0; y < clearedRowsCount; y++)
                {
                    gwp->SetCursor(clearedRows[y] - 4 + 1, 1);
                    
                    for (int x = 0; x < BOARD_WIDTH; x++)
                        *gwp << '-';
                }
                
                gwp->ClearReverseText();
            }
            
            if (timeInMs > pieceDropTimer)
            {
                pieceDropTimer = timeInMs + dropDelay;              
                
                if (gameGrid.CheckCollision(gameGrid.FallingPiece,
                                        gameGrid.FallingPiece->yPos + 1,
                                        gameGrid.FallingPiece->xPos))
                {
                    if (gameGrid.FallingPiece->yPos < 5)
                    {
                        // Game over!
                        gameOver = true;
                    }
                    else
                    {
                        // Set the piece into place
                        gameGrid.AbsorbPiece(gameGrid.FallingPiece,
                                             clearedRows,
                                             clearedRowsCount);                        
                        
                        if (clearedRowsCount > 0)
                        {
                            // Begin the animation
                            animationFrameCounter = totalFrameCounter;
                            animationStep = ANIMATION_ROWCLEAR_DASHES;
                            
                            // Increase the user's score
                            assert(clearedRowsCount <= 4);
                            gameState.AddToScore(LINE_SCORES[clearedRowsCount - 1]);
                        }
                    }

                }
                else
                    gameGrid.FallingPiece->yPos++;
            }
            
            // Refresh the screen
            gwp->Box();
            gwp->Refresh();
            
            // Handle post-rendering animation steps
            if (animationStep != ANIMATION_ROWCLEAR_NOTHING && 
                totalFrameCounter > animationFrameCounter + ANIMATION_FRAMES_PER_STEP)
            {
                if (animationStep == ANIMATION_ROWCLEAR_DASHES)
                {
                    // Set new animation step
                    animationStep = ANIMATION_ROWCLEAR_HIGHLIGHT;
                }
                else if (animationStep == ANIMATION_ROWCLEAR_HIGHLIGHT)
                {
                    animationStep = ANIMATION_ROWCLEAR_NOTHING;
                    
                    gameGrid.ClearRows(clearedRows, clearedRowsCount);
                }
                
                animationFrameCounter = totalFrameCounter;
            }
        }

        // Draw score window
        if (gameConsole.ScoreWindowPtr->IsVisible())
        {
            WindowWrapper *swp = gameConsole.ScoreWindowPtr; // Just an alias
            
            // Put the cursor at the beginning of the window's base coordinates
            swp->SetCursor(1,0);
            
            // Print the top score
            swp->PrintRow("TOP:", true);
            swp->PrintRow(to_string(gameState.GetTopScore()), true);
            *swp << endl;
            
            swp->PrintRow("SCORE:", true);
            swp->PrintRow(to_string(gameState.GetScore()), true);
            *swp << endl;
            
            // Print border & refresh
            swp->Box();
            swp->Refresh();
        }
        
        // Draw next piece window
        if (gameConsole.NextPieceWindowPtr->IsVisible())
        {
            WindowWrapper *nwp = gameConsole.NextPieceWindowPtr; // Just an alias
            
            // Clear the next piece window
            
            // Put the cursor at the beginning of the window's base coordinates
            nwp->SetCursor(1,0);
            
            nwp->SetBoldText();
            *nwp << " Next:" << endl << endl;
            nwp->ClearBoldText();
            
            for (int x = 2; x < NEXTPIECE_HEIGHT; x++)
                *nwp << endl;
                
            DrawPiece(nwp, gameGrid.NextPiece, true);
            
            nwp->Box();
            nwp->Refresh();
        }
        
        if (keyPress == Console::PRESSED_ESC)
            exitFlag = true;
                
        totalFrameCounter++;
    }
        
    return 0;
}

double get_time_ms()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}

void DrawPiece(WindowWrapper *wp, Piece *piece, bool nextPieceWindow)
{    
    for (int y = 0; y < piece->heightBox; y++)
    {
        // Print the piece
        // The 1 is added for the offset of the borders
        // The 4 is subtracted from y for the visible portion
        
        assert (NEXTPIECE_HEIGHT >= 3);
        
        if (nextPieceWindow)
            wp->SetCursor(y + (NEXTPIECE_HEIGHT - 3), 2);
        else
            wp->SetCursor(piece->yPos + y + 1 - 4,
                        piece->xPos + 1);
                       
        for (unsigned int x = 0; x < piece->widthBox; x++)
        {
            if ((*piece)[y][x] == 'X')
                *wp << (*piece)[y][x];
            else
                // Advance the position by 1
                if (nextPieceWindow)
                    wp->SetCursor(y + (NEXTPIECE_HEIGHT - 3),
                                   x + 3);
                else
                    wp->SetCursor(piece->yPos + y + 1 - 4,
                                   piece->xPos + 2 + x);
        }         
    }    
}
