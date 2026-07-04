#include "Viper.h"
#include "Bullet.h"
#include "BulletManager.h"
#include <cstdlib>

extern sf::Texture bulletTexture;

Viper::Viper(float startX, float startY, BulletManager* bm, sf::Texture& tex, float speedMult, float fireMult)
{
    bulletManager = bm;
    x = startX;
    y = startY;
    spawnX = startX;                               //Center of sine wave
    speed = 120.0f * speedMult;                    //Apply speed scaling
    vx = 0;                                        //Horizontal movement handled by sine formula
    vy = speed;                                    //Move downward
    sineOffset = (rand() % 628) / 100.0f;          //Random start position in wave (0 to 6.28)
    timeAlive = 0.0f;                              //Seconds since spawned
    waveAmplitude = 50.0f;                         //Swing 50 pixels left/right
    waveFrequency = 3.0f;                          //Speed of oscillation
    minShootTime = 1.5f / fireMult;                //Faster shooting with scaling
    maxShootTime = 3.5f / fireMult;                //Faster shooting with scaling
    health = 1;                                    //Dies in one hit
    points = 150;                                  //More points than Drone
    shape.setFillColor(sf::Color(255, 165, 0));    //Orange color
    shape.setSize(sf::Vector2f(width, height));    //Set hitbox size
    shape.setOrigin(width / 2, height / 2);        //Center the shape
    shape.setPosition(x, y);                       //Position at spawn location
    loadTexture(tex);                              //Load viper sprite
    shootInterval = minShootTime + (rand() % (int)((maxShootTime - minShootTime) * 100)) / 100.0f;  //Random first shot
    this->speedMult = speedMult;                   //Store for bullet speed scaling
}

void Viper::update(float deltaTime)
{
    timeAlive += deltaTime;  //Increase time for sine wave calculation

    //Calculate horizontal position using sine wave
    x = spawnX + sin(timeAlive * waveFrequency + sineOffset) * waveAmplitude;

    y += vy * deltaTime;  //Move downward

    //Keep Viper within screen boundaries
    if (x < width / 2)
        x = width / 2;
    if (x > 800 - width / 2)
        x = 800 - width / 2;

    //Handle shooting timer (from Enemy)
    if (shootTimer > 0)
        shootTimer -= deltaTime;

    if (shootTimer <= 0 && alive == true)
    {
        shoot();
        shootTimer = shootInterval;
    }

    shape.setPosition(x, y);  //Sync visual

    if (y > 600)  //Off bottom of screen
        alive = false;

    //Animate viper sprite
    if (hasSprite)
    {
        animationTimer += deltaTime;
        if (animationTimer > 0.2f)
        {
            currentFrame = (currentFrame + 1) % 3;
            sprite.setTextureRect(sf::IntRect(currentFrame * 200, 0, 200, 200));
            animationTimer = 0.0f;
        }
        sprite.setPosition(x, y);
    }
}

void Viper::shoot()
{
    float bulletSpeed = ENEMY_BULLET_SPEED * speedMult;
    Bullet* b = new Bullet(x, y + height / 2, 0, bulletSpeed, OWNER_ENEMY);  //Create enemy bullet at viper position
    b->loadTexture(bulletTexture);
    bulletManager->addBullet(b);  //Add to bullet array
    shootInterval = minShootTime + (rand() % (int)((maxShootTime - minShootTime) * 100)) / 100.0f;  //New random interval
}

void Viper::onCollision(GameObject* other)
{
    //Stub for now
}

void Viper::loadTexture(sf::Texture& tex)
{
    sprite.setTexture(tex);
    sprite.setTextureRect(sf::IntRect(0, 0, 200, 200));
    sprite.setOrigin(100, 100);
    sprite.setPosition(x, y);
    sprite.setScale(0.25f, 0.25f);  //Smaller than default
    hasSprite = true;
}