#pragma once
#include "GameObject.h"  //Entity is a type of GameObject, inherits from base
#include <SFML/Graphics.hpp>

class Entity : public GameObject
{
	//Variables accessible to child classes
protected:
	float x, y;     //Position on screen
	float vx, vy;   //Velocity(speed and direction)
	float width, height;   //Size of object
	bool alive;  	//Is object alive or dead?

public:
	Entity();  //default ctr
	virtual ~Entity(); //destructor
	float getX() const;  //Returns x position
	float getY() const;  //Returns y position
	bool isAlive() const;  //Returns alive status
	sf::FloatRect getHitbox() const;  //Returns rectangle used for collision detection
	void setPosition(float, float);  //Changes position
	void setVelocity(float, float);  //Changes velocity
	void setAlive(bool);   //Changes alive status
	void move(float deltaTime);  //Updates position based on velocity and time

	// Pure virtual functions — Entity remains abstract
	virtual void update(float deltaTime) = 0;  //Forces children to update
	virtual void draw(sf::RenderWindow& window) = 0;  //Forces children to draw
	virtual void onCollision(GameObject* other) = 0;  //Forces children to react when hit
};