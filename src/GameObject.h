#include <SFML/Graphics.hpp>
#pragma once

class GameObject
{
	//no data members, abstract class

public:
	//Forces every child to have an update function
	virtual void update(float deltaTime) = 0;

	//Forces every child to know how to draw itself
	virtual void draw(sf::RenderWindow& window) = 0;

	//Forces every child to react when hit
	virtual void onCollision(GameObject* other) = 0;

	//Proper cleanup when deleting derived objects
	virtual ~GameObject() {}
};
