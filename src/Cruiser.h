#pragma once
#include "Enemy.h"
#include "BulletManager.h"

class Cruiser : public Enemy
{
private:
    float speed;               //Horizontal sweep speed
    int direction;             //1 = moving right, -1 = moving left
    float attackTimer;         //Countdown between attacks
    float attackCooldown;      //Time between laser walls
    float gapX;                //Center X position of the safe gap
    BulletManager* bulletManager;  //Pointer to bullet manager
    sf::Sprite sprite;
    int currentFrame;
    float animationTimer;
    bool hasSprite;

public:
    Cruiser(float startX, float startY, BulletManager* bm);  //Constructor
    void update(float deltaTime) override;                     //Sweep + attack timer
    void draw(sf::RenderWindow& window) override;              //Draw boss
    void shoot() override;                                     //Not used — uses fireLaserWall instead
    void onCollision(GameObject* other) override;              //Stub
    void fireLaserWall();                                      //Creates bullet wall with safe gap
    void loadTexture(sf::Texture& tex);
}; 