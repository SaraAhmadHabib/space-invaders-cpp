#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

class BulletManager;
class EnemyManager;
class ArcadeManager;

class Player : public Entity
{
	// Variables
	bool empKeyPressed;  //Tracks if N was pressed last frame
	EnemyManager* enemyManager;
	ArcadeManager* arcadeManager;
	BulletManager* bulletManager;  //Pointer to shared bullet manager
	int lives;  //How many lives remaining
	int empCount;  //EMP bombs available
	int shieldHits;  //Shield durability (hits remaining)
	int currentWeapon;  //Active weapon (0=Standard, 1=Spread, 2=Piercing)
	float dashCooldown;   //Seconds until dash ready (0 = ready)
	float invincibleTimer;  //Seconds of invincibility after hit
	float shootCooldown;  //Seconds until can shoot again
	float speed;   //Movement speed (pixels/second)
	sf::RectangleShape shape;   //Visual representation (temporary)
	sf::Sprite sprite;              //Player sprite (replaces shape)
	int currentFrame;               //Current animation frame (0, 1, or 2)
	float animationTimer;           //Timer between frame switches
	sf::Sprite invincibilityOrb;                     //Glow orb sprite
	sf::Sprite shieldOrbSprite;                      //Shield orb sprite
	float shieldBreakTimer;  //Timer for shield break flash
	sf::Texture* defaultTexture;
	sf::Texture* spreadTex;
	sf::Texture* piercingTex;
	sf::Sprite shieldBreakSprite;
	bool shieldBreakAnimating;
	int shieldBreakFrame;
	float shieldBreakAnimTimer;

	// Constants
	const int MAX_LIVES = 3;                //Maximum lives player can have
	const int MAX_EMP = 3;                  //Maximum EMP bombs player can hold
	const int MAX_SHIELD = 2;               //Maximum shield durability (hits)
	const float DASH_DISTANCE = 100.0f;     //Pixels moved per dash
	const float DASH_COOLDOWN_TIME = 3.0f;  //Seconds between dashes
	const float INVINCIBLE_TIME = 1.5f;     //Seconds invincible after hit
	const float SHOOT_COOLDOWN_TIME = 0.25f; //Seconds between shots (4 shots/sec)
	const int EMP_BOSS_DAMAGE = 5;  //Damage EMP deals to bosses

public: 
	Player(BulletManager* bm, EnemyManager* em, ArcadeManager* am); //Sets starting position, size, lives, and all default values
	~Player();   //Cleanup (empty for now)
	void update(float deltaTime) override;   //Called every frame — handles input, cooldowns, movement, screen bounds
	void draw(sf::RenderWindow& window) override;   //Draws player shape to screen
	void loadInvincibilityOrb(sf::Texture& tex);     //Load orb texture
	void loadShieldOrbTexture(sf::Texture& tex);     //Load shield orb
	void onCollision(GameObject* other) override;   //Reacts when hit by enemy or bullet
	void handleInput();   //Reads keyboard, sets velocity, triggers actions
	void loadTexture(sf::Texture& tex);  //Load and setup player sprite
	void shoot();   //Creates a bullet at player position (prints "Bang!" for now)
	void takeDamage(int damage);   //Reduces shield or lives when hit
	void dash(int direction);   //Teleports player short distance in movement direction
	void activateEMP();   //Destroys all enemies on screen (stub for now)
	void reset();
	float getDashCooldown();  //Return current dash cooldown timer
	void setSpreadTexture(sf::Texture& tex);
	void setPiercingTexture(sf::Texture& tex);

	//Getters
	int getLives() const;   //Returns remaining lives
	int getEmpCount() const;   //Returns available EMP bombs
	int getShieldHits() const;   //Returns shield durability (hits remaining)
	int getCurrentWeapon() const;   //Returns active weapon (0=Standard, 1=Spread, 2=Piercing)
	bool isInvincible() const;   //Returns true if invincible timer is active
	bool isDashReady() const;   //Returns true if dash is ready to use

	//Setters for PowerUPS
	void addLife();   //Increases lives by 1 (max MAX_LIVES)
	void addEMP();   //Increases EMP count by 1 (max MAX_EMP)
	void setWeapon(int type);   //Changes current weapon (0=Standard, 1=Spread, 2=Piercing)
	void addShield();   //Gives player full shield (sets shieldHits to MAX_SHIELD)
	void loadShieldBreakTexture(sf::Texture& tex);
};

