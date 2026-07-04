#include "Entity.h"
#include <SFML/Graphics.hpp>

Entity::Entity()  //default ctr
{
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
	width = 0;
	height = 0;
	alive = true;
}

Entity:: ~Entity()  //dtr
{

}

float Entity::getX() const
{
	return x;
}

float Entity::getY() const
{
	return y;
}

bool Entity::isAlive() const
{
	return alive;
}

sf::FloatRect Entity::getHitbox() const
{
	// SFML screen coordinates, (0,0) is TOP-LEFT.
	// X increases RIGHT, Y increases DOWN.
	// To find TOP-LEFT corner from CENTER position:
	float left = x - (width / 2.0f);   // Subtract half-width to move LEFT
	float top = y - (height / 2.0f);   // Subtract half-height to move UP

	return sf::FloatRect(left, top, width, height); // Creates collision rectangle
}

void Entity::setPosition(float xnew, float ynew)
{
	x = xnew;
	y = ynew;
}

void Entity::setVelocity(float vxnew, float vynew)
{
	vx = vxnew;
	vy = vynew;
}

void Entity::setAlive(bool status)
{
	alive = status;
}

void Entity::move(float deltaTime)
{
	//distance = speed × time
	x += vx * deltaTime;   //Apply horizontal velocity
	y += vy * deltaTime;   //Apply vertical velocity
}