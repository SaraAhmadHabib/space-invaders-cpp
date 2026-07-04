#pragma once
#include "PowerUp.h"
#include <SFML/Graphics.hpp>

class PowerUpManager
{
private:
    PowerUp** powerUps;        //Array of power-up pointers
    int powerUpCount;          //Current number of power-ups
    int powerUpCapacity;       //Current array size

public:
    PowerUpManager();          //Constructor
    ~PowerUpManager();         //Destructor
    void addPowerUp(PowerUp* p);    //Adds power-up to array, grows by +5
    void removePowerUp(int index);  //Deletes power-up at index, shifts array
    void updateAll(float deltaTime);   //Updates all power-ups, removes dead ones
    void drawAll(sf::RenderWindow& window);   //Draws all power-ups
    PowerUp* getPowerUp(int index);     //Returns power-up at index
    int getCount();               //Returns powerUpCount
    void clearAll();          //Deletes all power-ups
};