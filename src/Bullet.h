#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

//constants 
const float BULLET_WIDTH = 6.0f;       //Width of bullet
const float BULLET_HEIGHT = 12.0f;     //Height of bullet
const float PLAYER_BULLET_SPEED = -500.0f;  //Moves upward
const float ENEMY_BULLET_SPEED = 300.0f;    //Moves downward
const int OWNER_PLAYER = 0;   //Player bullet tag
const int OWNER_ENEMY = 1;    //Enemy bullet tag

class Bullet : public Entity  //Bullet inherits position, velocity, hitbox from Entity
{
	int damage;   //How much damage it deals 1 (normal), 2 (piercing)
	bool pierce;  //Can it hit multiple enemies?
	bool hasPierced;  //Has bullet already pierced one enemy?
	int owner;   //Who fired it? 0 = Player, 1 = Enemy
	sf::RectangleShape shape;
	sf::Sprite sprite;               //Bullet sprite
	bool hasSprite;                  //True when sprite is loaded

public:
	void setDamage(int value);  //Sets damage value (for Piercing Module)
	Bullet(float startX, float startY, float velX, float velY, int ownerType); 
	~Bullet();  //Destructor
	void update(float deltaTime) override;  //Moves bullet, checks off-screen
	void draw(sf::RenderWindow& window) override;  //Draws bullet shape
	void onCollision(GameObject* other) override;  //Handles collision (stub)
	bool isOffScreen() const;  //Returns true if bullet left screen
	int getDamage() const;  //Returns damage value
	int getOwner() const;  //Returns owner tag
	bool canPierce() const;  //Returns pierce flag
	void setPierce(bool value);  //Sets pierce flag (for Piercing Module)
	bool getHasPierced() const;
	void setHasPierced(bool value);
	void loadTexture(sf::Texture& tex);  //Load bullet sprite (frame based on owner)
};