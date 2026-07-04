#include "Asteroid.h"
#include <cstdlib>


Asteroid::Asteroid(float startX, float startY, float size)
{
    x = startX;
    y = startY;
    speed = 60.0f + (rand() % 40);      //Random speed between 60 and 100
    vx = 0;
    vy = speed;
    width = size;
    height = size;
    alive = true;

    int gray = 100 + (rand() % 100);     //Random gray shade (100-199)
    shape.setFillColor(sf::Color(gray, gray, gray));
    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(x, y);
    hasSprite = false;     //No sprite loaded yet
    textureIndex = 0;      //Default frame
    currentFrame = 0;
    animTimer = 0.0f;
    totalFrames = 12;  //2 rows, 6 cols
}

void Asteroid::update(float deltaTime)
{
    move(deltaTime);                     //Float downward
    if (hasSprite)
    {
        animTimer += deltaTime;
        if (animTimer > 0.15f)
        {
            currentFrame = (currentFrame + 1) % totalFrames;
            int col = currentFrame % 6;
            int row = currentFrame / 6;
            sprite.setTextureRect(sf::IntRect(col * 320, row * 320, 320, 320));
            animTimer = 0.0f;
        }
        sprite.setPosition(x, y);
    }
    shape.setPosition(x, y);             //Sync visual

    if (y > 600 + height)                //Off bottom of screen
        alive = false;
}

void Asteroid::draw(sf::RenderWindow& window)
{
    if (hasSprite)
        window.draw(sprite);  //Draw sprite
    else
        window.draw(shape);   //Fallback
}

void Asteroid::onCollision(GameObject* other)
{
    //Stub - handled by CollisionManager
}

void Asteroid::loadTexture(sf::Texture& tex, float scaleMultiplier)
{
    int frameWidth = 320;
    int frameHeight = 320;

    sprite.setTexture(tex);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2, frameHeight / 2);
    sprite.setPosition(x, y);
    sprite.setScale(0.15f * scaleMultiplier, 0.15f * scaleMultiplier);
    sprite.setColor(sf::Color(200, 200, 200, 255));
    hasSprite = true;
}