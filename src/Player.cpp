#include "Player.h"
#include "Bullet.h"
#include "ArcadeManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "EnemyManager.h"
#include "Explosion.h"
extern sf::Sound empSound;

using namespace std;
#include "BulletManager.h"

extern sf::Texture bulletTexture;
bool empFlash = false;
extern sf::Texture explosionTexture;
extern void addExplosion(Explosion*);
extern sf::Sound shootSound;
extern sf::Sound explosionSound;
extern sf::Sound hitSound;
extern sf::Sound shieldBreakSound;


Player::Player(BulletManager* bm, EnemyManager* em, ArcadeManager* am)
{
	bulletManager = bm;
	enemyManager = em;
	arcadeManager = am;
	x = 400;
	y = 540;
	width = 55;
	height = 55;
	lives = 3;
	empCount = 1;
	shieldHits = 0;
	currentWeapon = 0;
	dashCooldown = 0.0f;
	invincibleTimer = 0.0f;
	shootCooldown = 0.0f;
	speed = 300.0f;
	shape.setSize(sf::Vector2f(width, height));
	shape.setFillColor(sf::Color::White);
	shape.setOrigin(width / 2, height / 2);
	shape.setPosition(x, y);
	shieldBreakTimer = 0.0f;
	currentFrame = 0;              //Start at first frame
	animationTimer = 0.0f;         //Reset animation timer
	defaultTexture = nullptr;
	spreadTex = nullptr;
	piercingTex = nullptr;
	shieldBreakAnimating = false;
	shieldBreakFrame = 0;
	shieldBreakAnimTimer = 0.0f;
}

Player::~Player()
{
	// Nothing to clean up yet
}

void Player::loadTexture(sf::Texture& tex)
{

	defaultTexture = &tex;
	sprite.setTexture(tex);
	sprite.setTextureRect(sf::IntRect(0, 0, 486, 474));
	sprite.setOrigin(486 / 2, 474 / 2);
	sprite.setScale(0.25f, 0.25f);
	sprite.setPosition(x, y);
}

void Player::loadShieldBreakTexture(sf::Texture& tex)
{
	shieldBreakSprite.setTexture(tex);
	shieldBreakSprite.setTextureRect(sf::IntRect(0, 0, 500, 500));
	shieldBreakSprite.setOrigin(250, 250);
	shieldBreakSprite.setScale(0.41f, 0.41f);
	shieldBreakSprite.setColor(sf::Color(255, 255, 255, 130));  //Semi-transparent
}

void Player::update(float deltaTime)
{
	handleInput();   // Read keyboard and set velocity
	move(deltaTime); // Update position based on velocity

	// Reduce cooldowns
	shootCooldown -= deltaTime;
	if (shootCooldown < 0)
		shootCooldown = 0;

	dashCooldown -= deltaTime;
	if (dashCooldown < 0)
		dashCooldown = 0;

	invincibleTimer -= deltaTime;
	if (invincibleTimer < 0)
		invincibleTimer = 0;

	if (shieldBreakTimer > 0)
		shieldBreakTimer -= deltaTime;

	// Keep player inside screen boundaries (800x600)
	if (x < width / 2)
		x = width / 2;         // Left edge

	if (x > 800 - width / 2)
		x = 800 - width / 2;  // Right edge

	if (y < 65 + height / 2)
		y = 65 + height / 2;   // Top edge

	if (y > 600 - height / 2)
		y = 600 - height / 2;     // Bottom edge


	shape.setPosition(x, y);     // Sync visual with logical position
	//Animate player sprite
	animationTimer += deltaTime;                                   //Increase timer

	if (animationTimer > 0.2f)
	{
		currentFrame = (currentFrame + 1) % 3;

		if (currentWeapon == 0)
		{
			//Standard: 3 cols, 1 row
			sprite.setTextureRect(sf::IntRect(currentFrame * 486, 0, 486, 474));
		}
		else
		{
			//Spread/Piercing: 2 cols, 2 rows
			int col = currentFrame % 2;
			int row = currentFrame / 2;
			sprite.setTextureRect(sf::IntRect(col * 200, row * 200, 200, 200));
		}
		animationTimer = 0.0f;
	}
	sprite.setPosition(x, y);
	if (shieldBreakAnimating)
	{
		shieldBreakAnimTimer += deltaTime;
		if (shieldBreakAnimTimer > 0.05f)
		{
			shieldBreakFrame++;
			if (shieldBreakFrame >= 12)
			{
				shieldBreakAnimating = false;
			}
			else
			{
				int col = shieldBreakFrame % 6;
				int row = shieldBreakFrame / 6;
				shieldBreakSprite.setTextureRect(sf::IntRect(col * 500, row * 500, 500, 500));
			}
			shieldBreakAnimTimer = 0.0f;
		}
		shieldBreakSprite.setPosition(x, y);
	}
}

void Player::draw(sf::RenderWindow& window)
{
		window.draw(sprite);

	if (invincibleTimer > 0)
	{
		invincibilityOrb.setPosition(x - 28, y - 15);
		window.draw(invincibilityOrb);
	}

		if (shieldBreakAnimating)
		{
			window.draw(shieldBreakSprite);
		}
		else if (shieldHits > 0)
		{
			shieldOrbSprite.setColor(sf::Color(255, 255, 255, 70));
			shieldOrbSprite.setPosition(x, y - 6);
			window.draw(shieldOrbSprite);
		}

}

void Player::onCollision(GameObject* other)
{
	//Empty for now
}

void Player::handleInput()
{
	vx = 0;  //Reset velocity to zero (no movement unless key pressed)
	vy = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		vx = -speed;  //Left arrow - move left

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		vx = speed;  //Right arrow - move right

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		vy = -speed;  //Up arrow - move up

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		vy = speed;  //Down arrow - move down

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootCooldown <= 0)
		shoot();  //Spacebar - shoot if cooldown ready

	//Dash checks - one per direction
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) &&
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		dashCooldown <= 0)
		dash(0);  //Left

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) &&
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
		dashCooldown <= 0)
		dash(1);  //Right

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) &&
		sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
		dashCooldown <= 0)
		dash(2);  //Up

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) &&
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
		dashCooldown <= 0)
		dash(3);  //Down

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
	{
		if (!empKeyPressed)           //Was NOT pressed last frame
		{
			activateEMP();            //Activate once
			empKeyPressed = true;     //Mark as pressed
		}
	}
	else
	{
		empKeyPressed = false;        //Key released — reset
	}
}

void Player::shoot()
{
	shootSound.play();
	if (currentWeapon == 0)  //Standard - single bullet straight up
	{
		Bullet* b = new Bullet(x, y - height / 2, 0, PLAYER_BULLET_SPEED, OWNER_PLAYER);
		b->loadTexture(bulletTexture);
		bulletManager->addBullet(b);
	}
	else if (currentWeapon == 1)  //Spread - three bullets in fan shape
	{
		Bullet* b1 = new Bullet(x, y - height / 2, 0, PLAYER_BULLET_SPEED, OWNER_PLAYER);        //Center
		b1->loadTexture(bulletTexture);
		Bullet* b2 = new Bullet(x, y - height / 2, -200, PLAYER_BULLET_SPEED, OWNER_PLAYER);      //Left
		b2->loadTexture(bulletTexture);
		Bullet* b3 = new Bullet(x, y - height / 2, 200, PLAYER_BULLET_SPEED, OWNER_PLAYER);       //Right
		b3->loadTexture(bulletTexture);
		bulletManager->addBullet(b1);
		bulletManager->addBullet(b2);
		bulletManager->addBullet(b3);
	}
	else if (currentWeapon == 2)  //Piercing - single bullet, double damage, passes through
	{
		Bullet* b = new Bullet(x, y - height / 2, 0, PLAYER_BULLET_SPEED, OWNER_PLAYER);
		b->loadTexture(bulletTexture);
		b->setPierce(true);          //Can pierce first enemy
		b->setDamage(2);             //Double damage
		bulletManager->addBullet(b);
	}

	cout << "Bang!" << endl;  //Placeholder sound
	shootCooldown = SHOOT_COOLDOWN_TIME;  //Reset cooldown
}

void Player::takeDamage(int damage)
{
	hitSound.play();

	if (invincibleTimer > 0)
	{
		// You are invincible. Ignore damage. Do nothing.
		return;
	}

	if (shieldHits > 0)
	{
		shieldHits = shieldHits - 1;
		if (shieldHits == 0)
		{
			shieldBreakAnimating = true;
			shieldBreakFrame = 0;
			shieldBreakAnimTimer = 0.0f;
			shieldBreakSound.play();  
		}
		cout << "Shield absorbed hit! Shield left: " << shieldHits << endl;
		return;
	}

	// If you reach here, you have NO shield AND are NOT invincible
	if (lives > 0)
	{
		lives = lives - 1;
		cout << "No shield! Lives left: " << lives << endl;
		invincibleTimer = INVINCIBLE_TIME;
	}
}

void Player::reset()
{
	lives = 3;
	empCount = 1;
	shieldHits = 0;
	currentWeapon = 0;
	dashCooldown = 0.0f;
	invincibleTimer = 0.0f;
	shootCooldown = 0.0f;
	x = 400;
	y = 540;
	shape.setPosition(x, y);
	currentFrame = 0;              //Reset to first frame
	animationTimer = 0.0f;         //Reset animation timer
	shieldBreakTimer = 0.0f;
	if (defaultTexture)
	{
		sprite.setTexture(*defaultTexture);
		sprite.setTextureRect(sf::IntRect(0, 0, 486, 474));
		sprite.setOrigin(486 / 2, 474 / 2);
		sprite.setScale(0.25f, 0.25f);
	}
}

void Player::dash(int direction)
{
	if (dashCooldown > 0)
		return;

	if (direction == 0)
		x -= DASH_DISTANCE;  //Left
	else if (direction == 1)
		x += DASH_DISTANCE;  //Right
	else if (direction == 2)
		y -= DASH_DISTANCE;  //Up
	else if (direction == 3)
		y += DASH_DISTANCE;  //Down

	vx = 0;  //Stop normal movement
	vy = 0;

	dashCooldown = DASH_COOLDOWN_TIME;
	invincibleTimer = 0.5f;
}

void Player::activateEMP()
{
	if (empCount > 0)
	{
		empCount -= 1;
		empSound.play();

		//Spawn explosion for each enemy before clearing
		if (enemyManager)
		{
			for (int i = 0; i < enemyManager->getCount(); i++)
			{
				Enemy* enemy = enemyManager->getEnemy(i);
				if (enemy && enemy->isAlive())
				{
					Explosion* exp = new Explosion(enemy->getX(), enemy->getY(), explosionTexture);
					addExplosion(exp);
					explosionSound.setVolume(20.0f);
					explosionSound.play();
				}
			}
		}

	


		//Trigger flash first
		empFlash = true;

		//Damage boss if active
		if (arcadeManager && arcadeManager->isBossActive())
		{
			Enemy* boss = arcadeManager->getBoss();
			if (boss)
			{
				boss->takeDamage(EMP_BOSS_DAMAGE);

				//If boss died from EMP, spawn explosions
				if (!boss->isAlive())
				{
					float bx = boss->getX();
					float by = boss->getY();

					int count = 8 + (rand() % 3);
					for (int i = 0; i < count; i++)
					{
						float offsetX = (rand() % 200) - 100;
						float offsetY = (rand() % 160) - 80;

						Explosion* exp = new Explosion(bx + offsetX, by + offsetY, explosionTexture);
						exp->setScale(0.6f, 0.6f);
						exp->setFrameTimer((rand() % 20) * 0.02f);
						addExplosion(exp);
						explosionSound.setVolume(60.0f);
						explosionSound.play();
						explosionSound.setVolume(20.0f);
					}
				}
			}
		}
		cout << "EMP Activated!" << endl;
	}
}

int Player::getLives() const
{
	return lives;
}

int Player::getEmpCount() const
{
	return empCount;
}

int Player::getShieldHits() const
{
	return shieldHits;
}

int Player::getCurrentWeapon() const
{
	return currentWeapon;
}

bool Player::isInvincible() const
{
	return (invincibleTimer > 0);
}

bool Player::isDashReady() const
{
	return (dashCooldown <= 0);
}

void Player::addLife()
{
	lives++;
	if (lives > MAX_LIVES)
		lives = MAX_LIVES;
}

void Player::addEMP()
{
	empCount++;
	if (empCount > MAX_EMP)
		empCount = MAX_EMP;
}

void Player::setWeapon(int type)
{
	currentWeapon = type;

	if (type == 0 && defaultTexture)
	{
		sprite.setTexture(*defaultTexture);
		sprite.setTextureRect(sf::IntRect(0, 0, 486, 474));
		sprite.setOrigin(486 / 2, 474 / 2);
		sprite.setScale(0.25f, 0.25f);
		sprite.setPosition(x, y);
	}
	else if (type == 1 && spreadTex)
	{
		sprite.setTexture(*spreadTex);
		sprite.setTextureRect(sf::IntRect(0, 0, 200, 210));
		sprite.setOrigin(100, 105);
		sprite.setScale(0.785f, 0.785f);
		sprite.setPosition(x, y);  //Negative = up, increase number to go higher
	}
	else if (type == 2 && piercingTex)
	{
		sprite.setTexture(*piercingTex);
		sprite.setTextureRect(sf::IntRect(0, 0, 200, 210));
		sprite.setOrigin(100, 105);
		sprite.setScale(0.785f, 0.785f);
		sprite.setPosition(x, y);  //Negative = up, increase number to go higher
	}

	
}
void Player::addShield()
{
	shieldHits = MAX_SHIELD;  //Always set to 2, regardless of current value
}

float Player::getDashCooldown()
{
	return dashCooldown;  //Return remaining cooldown
}

void Player::loadInvincibilityOrb(sf::Texture& tex)
{
	invincibilityOrb.setTexture(tex);
	invincibilityOrb.setTextureRect(sf::IntRect(0, 0, 500, 600));  //First orb - yellow
	invincibilityOrb.setOrigin(250, 300);                            //Center of 500x600
	invincibilityOrb.setScale(0.37f, 0.37f);                         //Adjust to player size
	invincibilityOrb.setPosition(x, y);
	invincibilityOrb.setColor(sf::Color(255, 255, 255, 20));
}

void Player::loadShieldOrbTexture(sf::Texture& tex)
{
	shieldOrbSprite.setTexture(tex);
	shieldOrbSprite.setTextureRect(sf::IntRect(0, 0, 500, 500));  //Full image, single orb
	shieldOrbSprite.setOrigin(250, 250);                            //Center of 500x500
	shieldOrbSprite.setScale(0.4f, 0.4f);                         //Same size
	shieldOrbSprite.setColor(sf::Color(255, 255, 255, 70));         //Same transparency
}

void Player::setSpreadTexture(sf::Texture& tex)
{
	spreadTex = &tex;
}

void Player::setPiercingTexture(sf::Texture& tex)
{
	piercingTex = &tex;
}