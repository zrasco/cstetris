#include "WindowWrapper.h"

WindowWrapper::WindowWrapper(unsigned short x,
                             unsigned short y,
                             unsigned short newWidth,
                             unsigned short newHeight)
{
    // Set window state variables
    xPos = x;
    yPos = y;
    width = newWidth;
    height = newHeight;
    
    window = newwin(height, width, y, x);
    isVisible = false;
    inReverse = false;
    inBlinking = false;
}

WindowWrapper::~WindowWrapper()
{
    delwin(window);
}

void WindowWrapper::Clear()
{
    wclear(window);
}

void WindowWrapper::Box()
{
    wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
    //box(window, '*', '*');
    //touchwin(window);
}

void WindowWrapper::Refresh()
{
    wrefresh(window);
}

void WindowWrapper::SetCursor(unsigned int y, unsigned int x)
{
    wmove(window, y, x);
}

void WindowWrapper::PrintRow(std::string contents, bool center)
{
    if (center)
    {
        if (inReverse)
        // Temporarily turn reverse text off for pad spaces
            wattroff(window, A_REVERSE);
            
        unsigned int padding = (width - contents.length()) / 2;
        
        for (int x = 0; x < padding; x++)
            *this << ' ';
            
        if (inReverse)
            wattron(window, A_REVERSE);
    }

    // Print the contents now
    *this << contents << endl;
}

void WindowWrapper::SetReverseText()
{
    inReverse = true;
    wattron(window, A_REVERSE);
}

void WindowWrapper::ClearReverseText()
{
    inReverse = false;
    wattroff(window, A_REVERSE);
}

void WindowWrapper::SetBoldText()
{
    inBlinking = true;
    wattron(window, A_BOLD);
}

void WindowWrapper::ClearBoldText()
{
    inBlinking = false;
    wattroff(window, A_BOLD);
}

// Getters and setters
void WindowWrapper::SetVisibility(bool makeVisible)
{
    this->isVisible = makeVisible;
}

bool WindowWrapper::IsVisible()
{
    return isVisible;
}

void WindowWrapper::SetXPos(unsigned short x) { xPos = x; }
unsigned short WindowWrapper::GetXPos() { return xPos; }

void WindowWrapper::SetYPos(unsigned short y) { yPos = y; }
unsigned short WindowWrapper::GetYPos() { return yPos; }

void WindowWrapper::SetHeight(unsigned short newHeight) { height = newHeight; }
unsigned short WindowWrapper::GetHeight() { return height; }

void WindowWrapper::SetWidth(unsigned short newWidth) { width = newWidth; }
unsigned short WindowWrapper::GetWidth() { return width; }

const WindowWrapper& operator<<(const WindowWrapper &screen, int value)
{
    wprintw(screen.window, "%d", value);
    
    return screen;
}

const WindowWrapper& operator<<(const WindowWrapper &screen, unsigned int value)
{
    wprintw(screen.window, "%d", value);
    
    return screen;
}

const WindowWrapper& operator<<(const WindowWrapper &screen, char value)
{
    wprintw(screen.window, "%c", value);
    
    return screen;
}

const WindowWrapper& operator<<(const WindowWrapper &screen, double value)
{
    wprintw(screen.window, "%f", value);
    
    return screen;
}

const WindowWrapper& operator<<(const WindowWrapper &screen, std::string value)
{
    wprintw(screen.window, "%s", value.c_str());
    
    return screen;
}
