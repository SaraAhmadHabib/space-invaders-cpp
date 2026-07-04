#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

class Asteroid : public Entity
{
private:
    float speed;                   //Downward float speed
    sf::RectangleShape shape;      //Visual representation (gray rectangle)
    sf::Sprite sprite;               //Asteroid sprite
    bool hasSprite;                  //True when sprite is loaded
    int textureIndex;
    int currentFrame;
    float animTimer;
    int totalFrames;

public:
    Asteroid(float startX, float startY, float size);  //Constructor — sets random gray color
    void update(float deltaTime) override;              //Floats downward
    void draw(sf::RenderWindow& window) override;       //Draws asteroid
    void loadTexture(sf::Texture& tex, float scaleMultiplier);  //Load asteroid sprite with specific frames
    void onCollision(GameObject* other) override;       //Stub - handled by CollisionManager
};