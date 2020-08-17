#ifndef CONSOLE_H
#define CONSOLE_H

#include <ncurses.h>
#include <string>
#include "WindowWrapper.h"

class Console
{
    // ASCII codes & result mappings
    static const unsigned char ASCII_ESC = 27;
    static const unsigned char ASCII_CR = 13;
    static const unsigned char ASCII_LF = 10;
    static const unsigned char ASCII_LEFT_BRACKET = '[';
    static const unsigned char ASCII_A = 'A';
    static const unsigned char ASCII_B = 'B';
    static const unsigned char ASCII_C = 'C';
    static const unsigned char ASCII_D = 'D';
    
public:

    // Abstracted key press results
    static const unsigned char PRESSED_NOTHING = 0;
    static const unsigned char PRESSED_ESC = 1;
    static const unsigned char PRESSED_UP = 2;
    static const unsigned char PRESSED_DOWN = 3;
    static const unsigned char PRESSED_LEFT = 4;
    static const unsigned char PRESSED_RIGHT = 5;
    static const unsigned char PRESSED_ENTER = 6;
    
    // Standard constructor
    Console(unsigned int, unsigned int);
    ~Console();
    
    // Member functions
    void ClearScreen();
    void Refresh();
    
    unsigned char GetKey() const;

    WindowWrapper *MenuWindowPtr;
    WindowWrapper *GridWindowPtr;
    WindowWrapper *NextPieceWindowPtr;
    WindowWrapper *ScoreWindowPtr;
    
    unsigned char LastKey;
};

#endif // CONSOLE_H
