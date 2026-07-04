#pragma once
#include "Enemy.h"

class BulletManager;

class Viper : public Enemy
{
    BulletManager* bulletManager;  //Pointer to shared bullet manager
    float speed;                   //Movement speed downward
    float sineOffset;              //Random start position in wave cycle
    float timeAlive;               //Seconds since spawned
    float waveAmplitude;           //How far left/right it swings
    float waveFrequency;           //How fast it swings
    float spawnX;                  //Original X position (center of wave)
    float minShootTime;            //Minimum random wait between shots
    float maxShootTime;            //Maximum random wait between shots
    float speedMult;

public:
    Viper(float startX, float startY, BulletManager* bm, sf::Texture& tex, float speedMult, float fireMult);  //Constructor - sets color orange, speed, sprite  //Constructor — sets color orange, speed, random offset
    void update(float deltaTime) override;                  //Sine wave movement
    void shoot() override;                                  //Fires one bullet downward
    void onCollision(GameObject* other) override;           //Stub for now
    void loadTexture(sf::Texture& tex) override;  //Override for smaller viper size
};