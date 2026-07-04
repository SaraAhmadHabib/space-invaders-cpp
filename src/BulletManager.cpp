#include "BulletManager.h"

BulletManager::BulletManager()
{
	bullets = nullptr;
	bulletCount = 0;
	bulletCapacity = 0;
}

BulletManager::~BulletManager()
{
	for (int i = 0; i < bulletCount; i++)
	{
		delete bullets[i];
	}
	delete[] bullets;
	bullets = nullptr;
	bulletCount = 0;
	bulletCapacity = 0;
}

void BulletManager::addBullet(Bullet* b)
{
	if (bulletCount == bulletCapacity)
	{
		int newCapacity = bulletCapacity + 5;           //Grow by 5
		Bullet** newArray = new Bullet * [newCapacity]; //Create larger array

		for (int i = 0; i < bulletCount; i++)           //Copy old pointers
			newArray[i] = bullets[i];

		delete[] bullets;                               //Delete old array
		bullets = newArray;                             //Update pointer
		bulletCapacity = newCapacity;                   //Update capacity
	}
	bullets[bulletCount] = b;                   //Add new bullet
	bulletCount = bulletCount + 1;                      //Increment count
}

void BulletManager::removeBullet(int index)
{
	delete bullets[index];                              //Free bullet object

	for (int i = index; i < bulletCount - 1; i++)       //Shift remaining bullets left
		bullets[i] = bullets[i + 1];

	bulletCount -= 1;                                   //Decrement count
}

void BulletManager::updateAll(float deltaTime)
{
	//Update all bullets - move them forward
	for (int i = 0; i < bulletCount; i++)
		bullets[i]->update(deltaTime);

	//Remove dead bullets - loop backward to avoid index issues
	for (int i = bulletCount - 1; i >= 0; i--)
	{
		if (!bullets[i]->isAlive())
			removeBullet(i);
	}
}

void BulletManager::drawAll(sf::RenderWindow& window)
{
	//Draw all active bullets
	for (int i = 0; i < bulletCount; i++)
		bullets[i]->draw(window);
}

Bullet* BulletManager::getBullet(int index)
{
	return bullets[index];
}

int BulletManager::getCount()
{
	return bulletCount;
}

void BulletManager::clearAll()
{
	for (int i = 0; i < bulletCount; i++)
		delete bullets[i];  //Delete each bullet object
	bulletCount = 0;  //Reset count — used when EMP activates
}