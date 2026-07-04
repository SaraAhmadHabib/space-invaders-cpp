#pragma once
#include "Enemy.h"
#include "BulletManager.h"

class Turret : public Enemy
{
    BulletManager* bulletManager;
    sf::Sprite sprite;
    bool hasSprite;
    int currentFrame;
    float animTimer;
    int turretIndex;  //0 = left, 1 = right

public:
    Turret(float startX, float startY, BulletManager* bm);
    void update(float deltaTime) override;  //Stub - position set by TwinCannons
    void draw(sf::RenderWindow& window) override;  //Draw turret
    void shoot() override;  //Fires bullet downward
    void onCollision(GameObject* other) override;  //Stub
    void loadTexture(sf::Texture& tex, int index);
};
