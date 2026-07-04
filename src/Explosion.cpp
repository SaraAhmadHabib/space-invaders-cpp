#include "Explosion.h"

int frameWidths[7] = { 286, 286, 286, 286, 286, 286, 284 };
int frameX[7] = { 0, 286, 572, 858, 1144, 1430, 1716 };

Explosion::Explosion(float startX, float startY, sf::Texture& tex)
{
    texture = &tex;
    x = startX;
    y = startY;
    currentFrame = 0;
    totalFrames = 7;
    frameTimer = 0.0f;
    frameDuration = 0.08f;  //~12 frames per second
    finished = false;
    frameWidth = 286;  //Exact frame width

    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidths[0], 400));
    sprite.setOrigin(frameWidth / 2, 200);
    sprite.setPosition(x, y);
    sprite.setScale(0.25f, 0.25f);  //Scale down
}

void Explosion::update(float deltaTime)
{
    if (finished) return;

    frameTimer += deltaTime;
    if (frameTimer >= frameDuration)
    {
        currentFrame++;
        if (currentFrame >= totalFrames)
        {
            finished = true;
            return;
        }
        sprite.setTextureRect(sf::IntRect(frameX[currentFrame] - 55, 0, frameWidths[currentFrame], 400));
        frameTimer = 0.0f;
    }
}

void Explosion::draw(sf::RenderWindow& window)
{
    if (!finished)
        window.draw(sprite);
}

bool Explosion::isFinished()
{
    return finished;
}

void Explosion:: setScale(float sx, float sy) 
{ 
    sprite.setScale(sx, sy); 
}
void Explosion:: setFrameTimer(float t) 
{ 
    frameTimer = t; 
}