#pragma once
#include "Entity.h"            //Enemy inherits from Entity

class Enemy : public Entity
{
	//Variables accessible to child classes
protected:
	int health; 	        //How many hits to kill
	int maxHealth; 	        //Starting health(for boss health bars)
	float shootTimer;	    //Countdown until next shot
	float shootInterval;  	//Time between shots
	int points;	             //Score awarded on death
	sf::RectangleShape shape;		//Visual representation
	sf::Sprite sprite;              //Enemy sprite (replaces shape)
	int currentFrame;               //Current animation frame
	float animationTimer;           //Timer between frame switches
	bool hasSprite;                 //True when sprite texture is loaded

public:
	Enemy();   //Sets default values
	virtual ~Enemy(); //Destructor
	virtual void loadTexture(sf::Texture& tex);  //Load and setup enemy sprite
	void update(float deltaTime) override;  //Override Entity's pure virtual
	void draw(sf::RenderWindow& window) override;  //Override Entity's pure virtual
	void onCollision(GameObject* other) override;  //Override Entity's pure virtual
	virtual void takeDamage(int damage);        //Reduces health, marks dead if ≤ 0
	bool isDead();                      //Returns health <= 0
	int getPoints();                //Returns points value
	int getHealth();              //Returns current health
	int getMaxHealth();          //Returns max health
	virtual void shoot() = 0;  //Pure virtual — each enemy MUST implement
};


