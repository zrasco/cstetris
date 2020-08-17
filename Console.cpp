#include "Console.h"
#include <cstdlib>

Console::Console(unsigned int width, unsigned int height)
{
    // Workaround for box not properly displaying on Bellagio
    setenv("TERM","gnome", 1);
    
    // Ncurses-related initialization
    initscr();
    timeout(1);
    noecho();
    cbreak();
    curs_set(0);
}

Console::~Console()
{
    endwin();
    
    delete MenuWindowPtr;
    delete GridWindowPtr;
    delete NextPieceWindowPtr;
    delete ScoreWindowPtr;
}

void Console::ClearScreen()
{
    clear();
}

void Console::Refresh()
{
    refresh();
}

unsigned char Console::GetKey() const
{
    unsigned int retval = PRESSED_NOTHING;
    
    // Get a keypress
    int inputCode = getch();
    
    if (inputCode == ASCII_ESC)
    {
        // Get another character. ERR means user pressed escape by itself
        inputCode = getch();
        
        if (inputCode == ASCII_LEFT_BRACKET)
        {
            inputCode = getch();
            
            switch (inputCode)
            {
                case ASCII_A:
                    retval = PRESSED_UP;
                    break;
                case ASCII_B:
                    retval = PRESSED_DOWN;
                    break;
                case ASCII_C:
                    retval = PRESSED_RIGHT;
                    break;
                case ASCII_D:
                    retval = PRESSED_LEFT;
                    break;
            }
        }
        else if (inputCode == ERR)
            // User pressed escape
            retval = PRESSED_ESC;
    }
    else if (inputCode == ASCII_CR || inputCode == ASCII_LF)
        retval = PRESSED_ENTER;
    
    return retval;
}
