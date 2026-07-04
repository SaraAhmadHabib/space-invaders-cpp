#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

//PowerUp types
const int POWERUP_SPREAD = 0;    //Spread Module - fires 3 bullets in fan shape
const int POWERUP_PIERCING = 1;  //Piercing Module - double damage, passes through enemy
const int POWERUP_SHIELD = 2;    //Energy Shield - absorbs 2 hits
const int POWERUP_EMP = 3;       //EMP Bomb - adds 1 EMP (max 3)

class PowerUp : public Entity
{
private:
    int type;                   //Which power-up (0=Spread, 1=Piercing, 2=Shield, 3=EMP)
    float speed;                //Downward float speed
    sf::RectangleShape shape;   //Visual representation
    sf::Sprite sprite;               //Power-up sprite (replaces shape)
    bool hasSprite;                  //True when sprite is loaded

public:
    PowerUp(float startX, float startY, int powerUpType);  //Constructor - sets color based on type
    void update(float deltaTime) override;                  //Floats downward, marks dead if off-screen
    void draw(sf::RenderWindow& window) override;           //Draws power-up shape
    void onCollision(GameObject* other) override;           //Stub
    int getType();                                          //Returns power-up type
    void applyEffect(class Player* player);                 //Applies effect to player on collection
    void loadTexture(sf::Texture& tex);  //Load and setup power-up sprite
};