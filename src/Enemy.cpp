#include "Enemy.h"


Enemy::Enemy()
{
	health = 1;
	maxHealth = 1;
	shootTimer = 0.0f;
	shootInterval = 2.0f;
	points = 100;
	alive = true;
	width = 30.0f;
	height = 30.0f;
	shape.setSize(sf::Vector2f(width, height));
	shape.setOrigin(width / 2, height / 2);
	shape.setPosition(x, y);
	hasSprite = false;             //No sprite loaded yet - will draw shape
	currentFrame = 0;              //Start at first animation frame
	animationTimer = 0.0f;         //Reset animation timer
}

Enemy:: ~Enemy()
{

}

void Enemy::loadTexture(sf::Texture& tex)
{
	sprite.setTexture(tex);                                    //Set the texture image
	sprite.setTextureRect(sf::IntRect(0, 0, 200, 200));       //First frame (200 wide, 200 tall)
	sprite.setOrigin(100, 100);                                //Center the sprite (half of 200)
	sprite.setPosition(x, y);                                  //Position at enemy's location
	sprite.setScale(0.30f, 0.30f);                             //Scale down to 30x30 pixels
	hasSprite = true;                                          //Mark that sprite is ready
}

void Enemy::update(float deltaTime)
{
	if (shootTimer > 0)
		shootTimer -= deltaTime;  //Count down

	if (shootTimer <= 0 && alive == true)  //Time to shoot
	{
		shoot();                         //Fire bullet
		shootTimer = shootInterval;      //Reset timer
	}

	move(deltaTime);                     //Update position
	shape.setPosition(x, y);             //Sync visual

	//Animate enemy sprite (if loaded)
	if (hasSprite)
	{
		animationTimer += deltaTime;                               //Increase timer
		if (animationTimer > 0.2f)                                 //Switch frame every 0.2 seconds
		{
			currentFrame = (currentFrame + 1) % 3;                 //Cycle 0→1→2→0
			sprite.setTextureRect(sf::IntRect(currentFrame * 200, 0, 200, 200));  //Show current frame
			animationTimer = 0.0f;                                 //Reset timer
		}
		sprite.setPosition(x, y);                                  //Sync sprite with hitbox
	}
}

void Enemy::draw(sf::RenderWindow& window)
{
	if (hasSprite)
		window.draw(sprite);  //Draw animated sprite
	else
		window.draw(shape);   //Fallback - draw colored rectangle
}

void Enemy::onCollision(GameObject* other)
{
	//Stub - handled by CollisionManager later
}

void Enemy::takeDamage(int damage)
{
	health -= damage;        //Reduce health
	if (health <= 0)
		alive = false;       //Mark dead if health depleted
}

bool Enemy::isDead()
{
	return health <= 0;  //True if health depleted
}

int Enemy::getPoints()
{
	return points;  //Score value awarded to player when this enemy dies
}

int Enemy::getHealth()
{
	return health;  //Return current health
}

int Enemy::getMaxHealth()
{
	return maxHealth;  //Return max health
}

