#include "Drone.h"
#include "Bullet.h"     //To create bullets in shoot()
#include <cstdlib>      //For random shoot intervals
#include <ctime>        //seed random
#include "BulletManager.h"

extern sf::Texture bulletTexture;

Drone::Drone(float startX, float startY, BulletManager* bm, sf::Texture& tex, float speedMult, float fireMult)
{
    bulletManager = bm;
    x = startX;
    y = startY;
    speed = 150.0f * speedMult;          //Apply speed scaling
    vx = 0;
    vy = speed;
    minShootTime = 1.0f / fireMult;       //Faster shooting (smaller interval)
    maxShootTime = 3.0f / fireMult;
    health = 1;
    points = 100;
    shape.setFillColor(sf::Color::Green);
    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(x, y);
    loadTexture(tex);
    shootInterval = minShootTime + (rand() % (int)((maxShootTime - minShootTime) * 100)) / 100.0f;
    this->speedMult = speedMult;
}

void Drone::shoot()
{
    float bulletSpeed = ENEMY_BULLET_SPEED * speedMult;
    Bullet* b = new Bullet(x, y + height / 2, 0, bulletSpeed, OWNER_ENEMY);  //Create enemy bullet at drone position
    b->loadTexture(bulletTexture);
    bulletManager->addBullet(b);  //Add to global bullet array
    shootInterval = minShootTime + (rand() % (int)((maxShootTime - minShootTime) * 100)) / 100.0f;  //New random interval for next shot
}

void Drone::update(float deltaTime)
{
    Enemy::update(deltaTime);  //Handles shoot timer, calls shoot() when ready, moves downward
    if (y > 600)  //Off bottom of screen
        alive = false;
}

void Drone::onCollision(GameObject* other)
{
    //Stub for now
}