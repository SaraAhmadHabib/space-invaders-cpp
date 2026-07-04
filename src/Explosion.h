#pragma once
#include <SFML/Graphics.hpp>

class Explosion
{
private:
    sf::Sprite sprite;
    sf::Texture* texture;
    int currentFrame;
    int totalFrames;
    float frameTimer;
    float frameDuration;
    bool finished;
    float x, y;
    float frameWidth;

public:
    Explosion(float startX, float startY, sf::Texture& tex);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool isFinished();
    void setScale(float sx, float sy);
    void setFrameTimer(float t);
};