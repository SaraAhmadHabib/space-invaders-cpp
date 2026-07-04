#pragma once
#include "Enemy.h"
#include "BulletManager.h"
#include "EnemyManager.h"

class Mothership : public Enemy
{
private:
    EnemyManager* enemyManager;  //For spawning seekers
    BulletManager* bulletManager;   //For spawning bullets 
    float laserChargeTimer;         //Countdown until laser fires
    float laserCooldown;            //Time between laser attacks
    bool laserWarning;              //True during warning phase
    float seekerSpawnTimer;         //Countdown until next seeker
    float seekerSpawnInterval;      //Time between seeker spawns
    float playerX;                  //Player's X position for seeker targeting
    sf::Sprite sprite;
    int currentFrame;
    float animationTimer;
    bool hasSprite;
    float warningFlashTimer;  //Timer for warning flash effect
    bool warningFlashActive;  //True when warning is visible
    bool sirenPlayed;

public:
    Mothership(float startX, float startY, BulletManager* bm, EnemyManager* em);   //Constructor
    void update(float deltaTime) override;                        //Manages laser + seekers
    void draw(sf::RenderWindow& window) override;                 //Draws mothership
    void shoot() override;                                        //Not used
    void onCollision(GameObject* other) override;                 //Stub
    void fireLaser();                                             //Screen-wide laser attack
    void spawnSeeker();                                           //Spawns a seeker enemy
    void setPlayerX(float px);                                    //Updates player position
    void loadTexture(sf::Texture& tex);
    bool isLaserWarning();
    float getWarningTimer();
    ~Mothership();
};