#include "Cruiser.h"
#include "Bullet.h"
#include <cstdlib>

extern sf::Texture bulletTexture;

Cruiser::Cruiser(float startX, float startY, BulletManager* bm)
{
	bulletManager = bm;
	x = startX;
	y = startY + 65;
	speed = 100.0f;                    //Sweep speed
	direction = 1;                     //Start moving right
	attackTimer = 3.0f;                //First attack after 3 seconds
	attackCooldown = 3.0f;             //Time between attacks
	health = 20;                       //Boss health
	maxHealth = 20;
	points = 1000;                     //Big score reward
	width = 80.0f;                     //Large boss
	height = 40.0f;
	vx = speed * direction;
	vy = 0;                            //Doesn't move down
	alive = true;

	shape.setFillColor(sf::Color(200, 0, 0));  //Dark red
	shape.setSize(sf::Vector2f(width, height));
	shape.setOrigin(width / 2, height / 2);
	shape.setPosition(x, y);
	hasSprite = false;
	currentFrame = 0;
	animationTimer = 0.0f;
}

void Cruiser::update(float deltaTime)
{
	//Sweep left and right
	x += vx * deltaTime;

	//Bounce at screen edges
	if (x > 800 - width / 2)
	{
		x = 800 - width / 2;
		direction = -1;
		vx = speed * direction;
	}
	if (x < width / 2)
	{
		x = width / 2;
		direction = 1;
		vx = speed * direction;
	}

	//Attack timer
	attackTimer -= deltaTime;
	if (attackTimer <= 0.0f)
	{
		fireLaserWall();
		attackTimer = attackCooldown;
	}

	shape.setPosition(x, y);

	if (hasSprite)
	{
		animationTimer += deltaTime;
		if (animationTimer > 0.15f)
		{
			currentFrame = (currentFrame + 1) % 8;
			sprite.setTextureRect(sf::IntRect(currentFrame * 512, 0, 512, 512));
			animationTimer = 0.0f;
		}
		sprite.setPosition(x, y);
	}
}

void Cruiser::fireLaserWall()
{
	gapX = 150.0f + (rand() % 500);  //Random gap position, stays on screen

	for (float bx = 40.0f; bx < 760.0f; bx += 40.0f)   //Edge to edge
	{
		if (bx > gapX - 70.0f && bx < gapX + 70.0f)    //140px safe gap
			continue;

		for (int row = 0; row < 2; row++)              //3 rows thick
		{
			Bullet* b = new Bullet(bx, y + height / 2 + row * 15.0f, 0, 300.0f, OWNER_ENEMY);
			b->loadTexture(bulletTexture);
			bulletManager->addBullet(b);
		}
	}
}

void Cruiser::onCollision(GameObject* other)
{
	//Stub — handled by CollisionManager
}

void Cruiser::draw(sf::RenderWindow& window)
{
	if (hasSprite)
		window.draw(sprite);
	else
		window.draw(shape);
}

void Cruiser::shoot()
{
	//Not used — Cruiser uses fireLaserWall() instead
}

void Cruiser::loadTexture(sf::Texture& tex)
{
	sprite.setTexture(tex);
	sprite.setTextureRect(sf::IntRect(0, 0, 512, 512));
	sprite.setOrigin(256, 256);
	sprite.setScale(0.2, 0.2f);
	sprite.setPosition(x, y);
	hasSprite = true;
}