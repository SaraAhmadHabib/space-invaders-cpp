#pragma once
#include "Bullet.h"
#include <SFML/Graphics.hpp>

class BulletManager
{
private:
    Bullet** bullets;          //Array of bullet pointers
    int bulletCount;           //Current number of bullets
    int bulletCapacity;        //Current array size

public:
    BulletManager();           //Constructor
    ~BulletManager();          //Destructor
    void addBullet(Bullet* b);    //Adds bullet to array, grows by +5
    void removeBullet(int index);  //Deletes bullet at index, shifts array
    void updateAll(float deltaTime);   //Updates all bullets, removes dead ones
    void drawAll(sf::RenderWindow& window);   //Draws all bullets
    Bullet* getBullet(int index);     //Returns bullet at index
    int getCount();               //Returns bulletCount
    void clearAll();          //Deletes all bullets (for EMP)
};