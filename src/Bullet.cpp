#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Bullet::Bullet(float startX, float startY, float velX, float velY, int ownerType)
{
	hasPierced = false;
	x = startX;
	y = startY;
	vx = velX;
	vy = velY;
	owner = ownerType;
	damage = 1;
	pierce = false;
	width = BULLET_WIDTH;
	height = BULLET_HEIGHT;
	alive = true;
	shape.setSize(sf::Vector2f(width, height));

	//	Set shape color: White for player, Red for enemy
	if (ownerType == OWNER_PLAYER)
		shape.setFillColor(sf::Color::White);
	else if (ownerType == OWNER_ENEMY)
		shape.setFillColor(sf::Color::Red);

	shape.setOrigin(width / 2, height / 2);
	shape.setPosition(x, y);
	hasSprite = false;
}

Bullet:: ~Bullet()
{

}

void Bullet::update(float deltaTime)
{
	move(deltaTime);
	if (isOffScreen())
		alive = false;
	shape.setPosition(x, y);
	if (hasSprite)
		sprite.setPosition(x, y);  //Sync sprite with hitbox
}

void Bullet::draw(sf::RenderWindow& window)
{
	if (hasSprite)
		window.draw(sprite);  //Draw sprite
	else
		window.draw(shape);   //Fallback - draw colored rectangle
}

void Bullet::onCollision(GameObject* other)
{
	//Empty for now
}

bool Bullet::isOffScreen() const
{
	//Check if bullet left the 800x600 screen
	return (y < 0 || y > 600 || x < 0 || x > 800);

}

int Bullet::getDamage() const
{
	return damage;
}

int Bullet::getOwner() const
{
	return owner;
}

bool Bullet::canPierce() const
{
	return pierce;
}

void Bullet::setPierce(bool value)
{
	pierce = value;
}

void Bullet::setDamage(int value)
{
	damage = value;  //Set damage amount
}

bool Bullet::getHasPierced() const 
{ 
	return hasPierced;
}
void Bullet::setHasPierced(bool value) 
{ 
	hasPierced = value; 
}

void Bullet::loadTexture(sf::Texture& tex)
{
	sprite.setTexture(tex);

	if (owner == OWNER_PLAYER)
		sprite.setTextureRect(sf::IntRect(0, 0, 66, 200));        //Frame 1: player bullet
	else
		sprite.setTextureRect(sf::IntRect(66, 0, 66, 200));       //Frame 2: enemy bullet

	sprite.setOrigin(33, 100);                                     //Center of 66x200 frame
	sprite.setPosition(x, y);
	sprite.setScale(0.30f, 0.30f);                                 //Small laser bolt
	hasSprite = true;
}