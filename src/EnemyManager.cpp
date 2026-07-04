#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	enemies = nullptr;
	enemyCount = 0;
	enemyCapacity = 0;
}

EnemyManager::~EnemyManager()
{
	for (int i = 0; i < enemyCount; i++)
		delete enemies[i];
	delete[] enemies;
	enemies = nullptr;
	enemyCount = 0;
	enemyCapacity = 0;
}

void EnemyManager::addEnemy(Enemy* e)
{
	if (enemyCount == enemyCapacity)
	{
		int newCapacity = enemyCapacity + 5;           //Grow by 5
		Enemy** newArray = new Enemy * [newCapacity];  //Create larger array

		for (int i = 0; i < enemyCount; i++)           //Copy old pointers
			newArray[i] = enemies[i];

		delete[] enemies;                               //Delete old array
		enemies = newArray;                             //Update pointer
		enemyCapacity = newCapacity;                    //Update capacity
	}
	enemies[enemyCount] = e;                   //Add new enemy
	enemyCount++;
}

void EnemyManager::removeEnemy(int index)
{
	delete enemies[index];                              //Free enemy object

	for (int i = index; i < enemyCount - 1; i++)       //Shift remaining enemies left
		enemies[i] = enemies[i + 1];

	enemyCount--;                                       //Decrement count
}

void EnemyManager::updateAll(float deltaTime)
{
	//Update all enemies - move them forward
	for (int i = 0; i < enemyCount; i++)
		enemies[i]->update(deltaTime);

	//Remove dead enemies - loop backward to avoid index issues
	for (int i = enemyCount - 1; i >= 0; i--)
	{
		if (!enemies[i]->isAlive())
			removeEnemy(i);
	}
}

void EnemyManager::drawAll(sf::RenderWindow& window)
{
	//Draw all active enemies
	for (int i = 0; i < enemyCount; i++)
		enemies[i]->draw(window);
}

Enemy* EnemyManager::getEnemy(int index)
{
	return enemies[index];
}

int EnemyManager::getCount()
{
	return enemyCount;
}

void EnemyManager::clearAll()
{
	for (int i = 0; i < enemyCount; i++)
		delete enemies[i];  //Delete each enemy object
	enemyCount = 0;  //Reset count — used when EMP activates
}
