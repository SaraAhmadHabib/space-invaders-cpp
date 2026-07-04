#pragma once
#include "Enemy.h"

class BulletManager;

class Drone : public Enemy
{
	BulletManager* bulletManager;
	float speed;              //Movement speed downward
	float minShootTime;       //Minimum random wait between shots
	float maxShootTime;       //Maximum random wait between shots
	float speedMult;  //Store speed multiplier for bullet speed

public:
	Drone(float startX, float startY, BulletManager* bm, sf::Texture& tex, float speedMult, float fireMult);        //Sets position, color (green), speed
	void update(float deltaTime) override;          //Moves straight down, wraps/removes when off-screen
	void shoot() override;                                   //Creates one red bullet moving downward
	void onCollision(GameObject* other) override;   //Stub for now
};
