#include "AsteroidManager.h"
#include <cstdlib>

extern sf::Texture asteroidTexture;
sf::Texture asteroidSmallTex;
sf::Texture asteroidLargeTex;
sf::Texture asteroidMiddleTex;
bool asteroidTexturesLoaded = false;

AsteroidManager::AsteroidManager()
{
    asteroids = nullptr;
    asteroidCount = 0;
    asteroidCapacity = 0;
    minAsteroids = 3;            //Minimum asteroids on screen
    maxAsteroids = 5;            //Maximum asteroids on screen
}

AsteroidManager::~AsteroidManager()
{
    for (int i = 0; i < asteroidCount; i++)
        delete asteroids[i];     //Delete each asteroid object
    delete[] asteroids;          //Delete the array
    asteroids = nullptr;
    asteroidCount = 0;
    asteroidCapacity = 0;
}

void AsteroidManager::addAsteroid(Asteroid* a)
{
    if (asteroidCount == asteroidCapacity)
    {
        int newCapacity = asteroidCapacity + 5;              //Grow by 5
        Asteroid** newArray = new Asteroid * [newCapacity];  //Create larger array

        for (int i = 0; i < asteroidCount; i++)              //Copy old pointers
            newArray[i] = asteroids[i];

        delete[] asteroids;                                  //Delete old array
        asteroids = newArray;                                //Update pointer
        asteroidCapacity = newCapacity;                      //Update capacity
    }
    asteroids[asteroidCount] = a;                    //Add new asteroid
    asteroidCount++;                                 //Increment count
}

void AsteroidManager::removeAsteroid(int index)
{
    delete asteroids[index];                                 //Free asteroid object

    for (int i = index; i < asteroidCount - 1; i++)         //Shift remaining asteroids left
        asteroids[i] = asteroids[i + 1];

    asteroidCount--;                                         //Decrement count
}

void AsteroidManager::loadTextures()
{
    smallTex.loadFromFile("assets/asteroidsmall.png");
    largeTex.loadFromFile("assets/asteroidlarge.png");
    middleTex.loadFromFile("assets/asteroidmiddle.png");
    texturesLoaded = true;
}

void AsteroidManager::updateAll(float deltaTime)
{
    //Update all asteroids - move them downward
    for (int i = 0; i < asteroidCount; i++)
        asteroids[i]->update(deltaTime);

    //Remove dead asteroids (off-screen) - loop backward
    for (int i = asteroidCount - 1; i >= 0; i--)
    {
        if (!asteroids[i]->isAlive())
            removeAsteroid(i);
    }

    //Maintain 3-5 asteroids on screen at all times
    while (asteroidCount < minAsteroids)
    {
        float randomX = 50.0f + (rand() % 700);       //Random X position
        float randomSize = 20.0f + (rand() % 30);      //Random size between 20 and 50
        Asteroid* a = new Asteroid(randomX, -50.0f, randomSize);
        if (!texturesLoaded) loadTextures();
        int pick = rand() % 3;
        if (pick == 0)      a->loadTexture(smallTex, 4.0f);
        else if (pick == 1) a->loadTexture(largeTex, 1.5f);
        else                a->loadTexture(middleTex, 2.5f);            
        addAsteroid(a);
    }
}

void AsteroidManager::drawAll(sf::RenderWindow& window)
{
    //Draw all active asteroids
    for (int i = 0; i < asteroidCount; i++)
        asteroids[i]->draw(window);
}

Asteroid* AsteroidManager::getAsteroid(int index)
{
    return asteroids[index];
}

int AsteroidManager::getCount()
{
    return asteroidCount;
}

void AsteroidManager::clearAll()
{
    for (int i = 0; i < asteroidCount; i++)
        delete asteroids[i];  //Delete each asteroid object
    asteroidCount = 0;         //Reset count
}