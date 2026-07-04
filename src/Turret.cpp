#include "Turret.h"
#include "Bullet.h"
#include "BulletManager.h"

extern sf::Texture bulletTexture;

Turret::Turret(float startX, float startY, BulletManager* bm)
{
    bulletManager = bm;
    x = startX;
    y = startY;
    health = 10;                       //Turret health
    maxHealth = 10;
    points = 0;                        //Points awarded to main boss
    width = 60.0f;    
    height = 200.0f;   
    shootInterval = 1.5f;              //Shoot every 1.5 seconds
    shootTimer = 1.0f;                 //First shot after 1 second
    vx = 0;
    vy = 0;
    alive = true;

    shape.setFillColor(sf::Color(255, 100, 0));  //Orange-red
    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(x, y);
    hasSprite = false;
    currentFrame = 0;
    animTimer = 0.0f;
    turretIndex = 0;
}

void Turret::loadTexture(sf::Texture& tex, int index)
{
    turretIndex = index;
    int frameWidth = 250;   //3500/14
    int frameHeight = 575;  //1150/2

    sprite.setTexture(tex);
    sprite.setTextureRect(sf::IntRect(index * frameWidth, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2, frameHeight / 2);
    sprite.setPosition(x, y);
    sprite.setScale(0.60f, 0.60f);  //Scale down
    hasSprite = true;
    currentFrame = turretIndex;  //Start at correct first frame
}

void Turret::update(float deltaTime)
{
    //Position updated by TwinCannons - just handle shooting
    if (shootTimer > 0)
        shootTimer -= deltaTime;

    if (shootTimer <= 0 && alive)
    {
        shoot();
        shootTimer = shootInterval;
    }

    shape.setPosition(x, y);  //Same as sprite position  

    if (hasSprite)
    {
        animTimer += deltaTime;
        if (animTimer > 0.2f)
        {
            currentFrame += 2;
            if (currentFrame >= 14)
                currentFrame = turretIndex;

            sprite.setTextureRect(sf::IntRect(currentFrame * 250, 0, 250, 575));
            animTimer = 0.0f;
        }
        sprite.setPosition(x, y);
    }
}

void Turret::draw(sf::RenderWindow& window)
{
    if (hasSprite)
        window.draw(sprite);
    else
        window.draw(shape);
}

void Turret::shoot()
{
    Bullet* b = new Bullet(x, y + 90, 0, 300.0f, OWNER_ENEMY);
    b->loadTexture(bulletTexture);
    bulletManager->addBullet(b);
}

void Turret::onCollision(GameObject* other)
{
    //Stub
}