#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>

class EnemyManager
{
private:
    Enemy** enemies;          //Array of enemy pointers
    int enemyCount;           //Current number of enemies
    int enemyCapacity;        //Current array size

public:
    EnemyManager();           //Constructor — sets to null, counts to 0
    ~EnemyManager();          //Destructor — deletes all enemies and array
    void addEnemy(Enemy* e);    //Adds enemy to array, grows by +5
    void removeEnemy(int index);  //Deletes enemy at index, shifts array
    void updateAll(float deltaTime);   //Updates all enemies, removes dead ones
    void drawAll(sf::RenderWindow& window);   //Draws all enemies
    Enemy* getEnemy(int index);     //Returns enemy at index
    int getCount();               //Returns enemyCount
    void clearAll();          //Deletes all enemies (for EMP)
};