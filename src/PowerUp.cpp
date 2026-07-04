#include "PowerUp.h"
#include "Player.h"
#include <iostream>

PowerUp::PowerUp(float startX, float startY, int powerUpType)
{
    type = powerUpType;
    x = startX;
    y = startY;
    speed = 120.0f;                         //Slow float downward
    vx = 0;
    vy = speed;
    width = 20.0f;
    height = 20.0f;
    alive = true;

    //Set color based on power-up type
    if (type == POWERUP_SPREAD)
        shape.setFillColor(sf::Color(255, 255, 0));     //Yellow - Spread
    else if (type == POWERUP_PIERCING)
        shape.setFillColor(sf::Color(255, 105, 180));     //Hot pink - Piercing
    else if (type == POWERUP_SHIELD)
        shape.setFillColor(sf::Color(0, 0, 255));        //Blue - Shield
    else if (type == POWERUP_EMP)
        shape.setFillColor(sf::Color(255, 0, 255));     //Magenta - EMP

    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(x, y);
    hasSprite = false;  //No sprite loaded yet
}

void PowerUp::loadTexture(sf::Texture& tex)
{
    sprite.setTexture(tex);                                    //Set the texture image
    sprite.setTextureRect(sf::IntRect(0, 0, 200, 200));       //Single frame (200x200)
    sprite.setOrigin(100, 100);                                //Center the sprite
    sprite.setPosition(x, y);                                  //Position at power-up location
    sprite.setScale(0.25f, 0.25f);                             //Scale down to ~24x24 pixels
    hasSprite = true;                                          //Mark that sprite is ready
}

void PowerUp::update(float deltaTime)
{
    move(deltaTime);                     //Float downward
    if (y > 600)                         //Off bottom of screen
        alive = false;
    shape.setPosition(x, y);             //Sync visual
    if (hasSprite)
        sprite.setPosition(x, y);  //Sync sprite position
}

void PowerUp::draw(sf::RenderWindow& window)
{
    if (hasSprite)
        window.draw(sprite);  //Draw sprite
    else
        window.draw(shape);   //Fallback - draw colored rectangle
}

void PowerUp::onCollision(GameObject* other)
{
    //Stub - handled by CollisionManager
}

int PowerUp::getType()
{
    return type;  //Return power-up type
}

void PowerUp::applyEffect(Player* player)
{
    if (type == POWERUP_SPREAD)
        player->setWeapon(1);             //Switch to Spread
    else if (type == POWERUP_PIERCING)
        player->setWeapon(2);             //Switch to Piercing
    else if (type == POWERUP_SHIELD)
    {
        player->addShield();  //Add shield
        std::cout << "Shield collected!" << std::endl;
    }           
    else if (type == POWERUP_EMP)
        player->addEMP();                 //Add EMP
}