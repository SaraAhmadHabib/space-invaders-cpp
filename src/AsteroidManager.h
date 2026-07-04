#pragma once
#include "Asteroid.h"
#include <SFML/Graphics.hpp>

class AsteroidManager
{
private:
    Asteroid** asteroids;        //Array of asteroid pointers
    int asteroidCount;           //Current number of asteroids
    int asteroidCapacity;        //Current array size
    int minAsteroids;            //Minimum asteroids on screen (3)
    int maxAsteroids;            //Maximum asteroids on screen (5)
    sf::Texture smallTex;
    sf::Texture largeTex;
    sf::Texture middleTex;
    bool texturesLoaded;

public:
    AsteroidManager();           //Constructor
    ~AsteroidManager();          //Destructor
    void addAsteroid(Asteroid* a);    //Adds asteroid to array, grows by +5
    void removeAsteroid(int index);   //Deletes asteroid at index
    void updateAll(float deltaTime);  //Updates all, removes dead, spawns new ones
    void drawAll(sf::RenderWindow& window);  //Draws all asteroids
    Asteroid* getAsteroid(int index);  //Returns asteroid at index
    int getCount();               //Returns asteroidCount
    void clearAll();              //Deletes all asteroids
    void loadTextures();
};