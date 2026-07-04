#pragma once
#include "Enemy.h"
#include "Turret.h"
#include "BulletManager.h"

class TwinCannons : public Enemy
{
private:
    Turret* leftTurret;          //Left turret (composition)
    Turret* rightTurret;         //Right turret (composition)
    BulletManager* bulletManager;
    float turretOffsetX;         //Distance from center to turrets
    float turretOffsetY;         //Vertical offset for turrets
    bool coreVulnerable;         //True when both turrets destroyed
    sf::Sprite sprite;
    int currentFrame;
    float animationTimer;
    bool hasSprite;

public:
    TwinCannons(float startX, float startY, BulletManager* bm);  //Constructor
    void loadTurretTexture(sf::Texture& tex);                     //NEW - loads turret sprites
    ~TwinCannons();                                               //Destructor - deletes turrets
    void update(float deltaTime) override;                        //Updates turrets, checks core
    void draw(sf::RenderWindow& window) override;                 //Draws core + turrets
    void shoot() override;                                        //Not used - turrets shoot
    void onCollision(GameObject* other) override;                 //Stub
    void takeDamage(int damage) override;                         //Override - handles core immunity
    void loadTexture(sf::Texture& tex);

    Turret* getLeftTurret();      //Returns left turret
    Turret* getRightTurret();     //Returns right turret
    bool isCoreVulnerable();      //True when both turrets dead
};