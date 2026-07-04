#pragma once
#include "Enemy.h"

class BulletManager;

class Seeker : public Enemy
{
    BulletManager* bulletManager;  //Pointer to shared bullet manager
    float targetX;                 //Player's X position at spawn (lock-on)
    float speed;                   //Starting downward speed
    float acceleration;            //Increases speed over time

public:
    Seeker(float startX, float startY, float playerX, BulletManager* bm, sf::Texture& tex, float speedMult);  //Constructor - sets color red, locks onto player
    void update(float deltaTime) override;  //Accelerates downward toward targetX
    void shoot() override;                  //Empty — Seeker does NOT shoot
    void loadTexture(sf::Texture& tex) override;  //Override for 512x512 frames
    void onCollision(GameObject* other) override;  //Stub for now
};