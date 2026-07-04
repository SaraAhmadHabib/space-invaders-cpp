#include "PowerUpManager.h"

PowerUpManager::PowerUpManager()
{
    powerUps = nullptr;
    powerUpCount = 0;
    powerUpCapacity = 0;
}

PowerUpManager::~PowerUpManager()
{
    for (int i = 0; i < powerUpCount; i++)
        delete powerUps[i];
    delete[] powerUps;
    powerUps = nullptr;
    powerUpCount = 0;
    powerUpCapacity = 0;
}

void PowerUpManager::addPowerUp(PowerUp* p)
{
    if (powerUpCount == powerUpCapacity)
    {
        int newCapacity = powerUpCapacity + 5;              //Grow by 5
        PowerUp** newArray = new PowerUp * [newCapacity];   //Create larger array

        for (int i = 0; i < powerUpCount; i++)              //Copy old pointers
            newArray[i] = powerUps[i];

        delete[] powerUps;                                  //Delete old array
        powerUps = newArray;                                //Update pointer
        powerUpCapacity = newCapacity;                      //Update capacity
    }
    powerUps[powerUpCount] = p;                    //Add new power-up
    powerUpCount++;                                //Increment count
}

void PowerUpManager::removePowerUp(int index)
{
    delete powerUps[index];                                 //Free power-up object

    for (int i = index; i < powerUpCount - 1; i++)         //Shift remaining power-ups left
        powerUps[i] = powerUps[i + 1];

    powerUpCount--;                                         //Decrement count
}

void PowerUpManager::updateAll(float deltaTime)
{
    //Update all power-ups - move them downward
    for (int i = 0; i < powerUpCount; i++)
        powerUps[i]->update(deltaTime);

    //Remove dead power-ups - loop backward to avoid index issues
    for (int i = powerUpCount - 1; i >= 0; i--)
    {
        if (!powerUps[i]->isAlive())
            removePowerUp(i);
    }
}

void PowerUpManager::drawAll(sf::RenderWindow& window)
{
    //Draw all active power-ups
    for (int i = 0; i < powerUpCount; i++)
        powerUps[i]->draw(window);
}

PowerUp* PowerUpManager::getPowerUp(int index)
{
    return powerUps[index];
}

int PowerUpManager::getCount()
{
    return powerUpCount;
}

void PowerUpManager::clearAll()
{
    for (int i = 0; i < powerUpCount; i++)
        delete powerUps[i];  //Delete each power-up object
    powerUpCount = 0;         //Reset count
}