#include "Seeker.h"

Seeker::Seeker(float startX, float startY, float playerX, BulletManager* bm, sf::Texture& tex, float speedMult)
{
    bulletManager = bm;
    x = startX;
    y = startY;
    targetX = playerX;
    speed = 200.0f * speedMult;           //Apply speed scaling
    acceleration = 50.0f * speedMult;
    vx = 0;
    vy = speed;                                 //Start moving downward
    health = 1;
    points = 200;                               //More points than Drone
    shape.setFillColor(sf::Color::Red);         //Danger color
    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(x, y);
    loadTexture(tex);  //Load seeker sprite animation
}

void Seeker::loadTexture(sf::Texture& tex)
{
    sprite.setTexture(tex);
    sprite.setTextureRect(sf::IntRect(0, 0, 512, 512));       //Seeker frames are 512x512
    sprite.setOrigin(256, 256);                                //Center of 512x512 frame
    sprite.setPosition(x, y);
    sprite.setScale(0.22f, 0.22f);                             //Scale to ~25x25
    hasSprite = true;
}

void Seeker::update(float deltaTime)
{
    speed += acceleration * deltaTime;  //Get faster over time
    vy = speed;                         //Update downward velocity

    x += (targetX - x) * 0.02f;         //Steer toward locked position
    y += vy * deltaTime;                //Move downward

    //Keep Seeker within screen boundaries
    if (x < width / 2)
        x = width / 2;
    if (x > 800 - width / 2)
        x = 800 - width / 2;

    shape.setPosition(x, y);            //Sync visual

    //Animate seeker sprite
    if (hasSprite)
    {
        animationTimer += deltaTime;
        if (animationTimer > 0.2f)
        {
            currentFrame = (currentFrame + 1) % 3;
            sprite.setTextureRect(sf::IntRect(currentFrame * 512, 0, 512, 512));
            animationTimer = 0.0f;
        }
        sprite.setPosition(x, y);
    }

    if (y > 600)                        //Off bottom of screen
        alive = false;
}

void Seeker::shoot()
{
    //Seeker does not shoot
}

void Seeker::onCollision(GameObject* other)
{
    //Stub for now
}