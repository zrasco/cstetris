#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>

class Gamestate
{
public:
    // Constructor
    Gamestate();
    
    static const unsigned char MENU = 0;
    static const unsigned char PLAY = 1;
    
    static const unsigned int MAX_SPEED = 20;
    static const unsigned int MAX_LEVEL = 10;
    
    // Mode
    void SetMode(unsigned char);
    unsigned char GetMode();
    
    // Speed - decreases delay in which the blocks fall
    void SetSpeed(unsigned int);
    unsigned int GetSpeed();    
    
    // Level - increases the # of blocks you start with
    void SetLevel(unsigned int);
    unsigned int GetLevel();
    
    // Score - changes the player score
    void ResetScore();
    void AddToScore(unsigned int);
    unsigned int GetTopScore();
    unsigned int GetScore();
    
private:
    unsigned char mode;
    unsigned int speed;
    unsigned int level;
    unsigned int score;
};

#endif // GAMESTATE_H
