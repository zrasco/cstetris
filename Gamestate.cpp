#include "Gamestate.h"
#include <stdexcept>

Gamestate::Gamestate()
{
    SetLevel(0);
    SetSpeed(0);
    SetMode(MENU);
    ResetScore();
}
void Gamestate::SetMode(unsigned char newMode)
{
    mode = newMode;
}
unsigned char Gamestate::GetMode()
{
    return mode;
}

void Gamestate::SetSpeed(unsigned int newSpeed)
{
    if (newSpeed <= MAX_SPEED)
        speed = newSpeed;
}

unsigned int Gamestate::GetSpeed()
{
    return speed;
}

void Gamestate::SetLevel(unsigned int newLevel)
{
    if (newLevel <= MAX_LEVEL)
        level = newLevel;
}

unsigned int Gamestate::GetLevel()
{
    return level;
}

void Gamestate::ResetScore()
{
    score = 0;
}

void Gamestate::AddToScore(unsigned int points)
{
    score += points;
}

unsigned int Gamestate::GetScore()
{
    return score;
}

unsigned int Gamestate::GetTopScore()
{
    return 10000;
}
