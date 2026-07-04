#include "ScoreManager.h"

ScoreManager::ScoreManager()
{
    score = 0;
    multiplier = 1;
    chainTimer = 0.0f;
}

void ScoreManager::addKill(int points)
{
    score += points * multiplier;  //Add points with multiplier

    chainTimer = CHAIN_WINDOW;      //Reset chain window

    if (multiplier < 4)
        multiplier *= 2;            //1x → 2x → 4x
}

void ScoreManager::reset()
{
    score = 0;
    multiplier = 1;
    chainTimer = 0.0f;
}

void ScoreManager::update(float deltaTime)
{
    if (chainTimer > 0)
        chainTimer -= deltaTime;    //Count down chain window
    else
        multiplier = 1;             //Reset if window expired
}

void ScoreManager::resetMultiplier()
{
    multiplier = 1;                 //Reset on player damage
    chainTimer = 0.0f;
}

int ScoreManager::getScore() const
{
    return score;
}

int ScoreManager::getMultiplier() const
{
    return multiplier;
}