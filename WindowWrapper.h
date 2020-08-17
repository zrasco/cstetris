#ifndef WINDOWWRAPPER_H
#define WINDOWWRAPPER_H

#include <ncurses.h>
#include <string>

// Used to print endl like you would with cout
const char endl = '\n';

class WindowWrapper
{
public:
    WindowWrapper(unsigned short, unsigned short, unsigned short, unsigned short);
    ~WindowWrapper();
    
    void SetCursor(unsigned int, unsigned int);
    void Clear();
    void Box();
    void Refresh();
    void SetReverseText();
    void ClearReverseText();
    void SetBoldText();
    void ClearBoldText();
    void PrintRow(std::string, bool = false);
    
    // Visibility
    void SetVisibility(bool);
    bool IsVisible();
    
    // Getters and setters
    void SetXPos(unsigned short);
    unsigned short GetXPos();
    
    void SetYPos(unsigned short);
    unsigned short GetYPos();
    
    void SetHeight(unsigned short);
    unsigned short GetHeight();
    
    void SetWidth(unsigned short);
    unsigned short GetWidth();
    
    // Operator overloads
    friend const WindowWrapper& operator<<(const WindowWrapper&, int);
    friend const WindowWrapper& operator<<(const WindowWrapper&, unsigned int);
    friend const WindowWrapper& operator<<(const WindowWrapper&, double);
    friend const WindowWrapper& operator<<(const WindowWrapper&, char);
    friend const WindowWrapper& operator<<(const WindowWrapper&, std::string);
    
private:
    WINDOW *window;
    bool isVisible;
    bool inReverse;
    bool inBlinking;
    unsigned short xPos, yPos, width, height;
};

#endif  // WINDOWWRAPPER_H
