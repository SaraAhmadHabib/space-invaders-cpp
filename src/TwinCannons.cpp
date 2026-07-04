#include "TwinCannons.h"
#include "Bullet.h"

TwinCannons::TwinCannons(float startX, float startY, BulletManager* bm)
{
    bulletManager = bm;
    x = startX;
    y = startY + 80;
    health = 30;                       //Core health
    maxHealth = 30;
    points = 2000;
    width = 40.0f;                     //Core size
    height = 80.0f;
    turretOffsetX = 45.0f;             //Distance from center to turrets
    turretOffsetY = -35.0f;             //Below core
    coreVulnerable = false;
    alive = true;

    //Create turrets (composition - owned by this boss)
    leftTurret = new Turret(x - turretOffsetX, y + turretOffsetY, bulletManager);
    rightTurret = new Turret(x + turretOffsetX, y + turretOffsetY, bulletManager);

    shape.setFillColor(sf::Color(150, 0, 0));  //Dark red core
    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(x, y);
    hasSprite = false;
    currentFrame = 0;
    animationTimer = 0.0f;
}

TwinCannons::~TwinCannons()
{
    delete leftTurret;   //Clean up turrets
    delete rightTurret;
    leftTurret = nullptr;
    rightTurret = nullptr;
}

void TwinCannons::update(float deltaTime)
{
    //Update turret positions to follow core
    leftTurret->setPosition(x - turretOffsetX, y + turretOffsetY);
    rightTurret->setPosition(x + turretOffsetX, y + turretOffsetY);

    //Update turrets (handles their shooting)
    if (leftTurret->isAlive())
        leftTurret->update(deltaTime);
    if (rightTurret->isAlive())
        rightTurret->update(deltaTime);

    //Check if core becomes vulnerable
    if (!leftTurret->isAlive() && !rightTurret->isAlive())
        coreVulnerable = true;

    shape.setPosition(x, y);
    //In update():
    if (hasSprite)
    {
        animationTimer += deltaTime;
        if (animationTimer > 0.15f)
        {
            currentFrame++;
            if (currentFrame > 24)  //5 rows × 5 cols = 25 frames, stop at 24
                currentFrame = 0;

            int col = 2 + (currentFrame % 5);   //Start from col 2, wrap within 5 cols
            int row = currentFrame / 5;          //0 to 4 (5 rows)

            sprite.setTextureRect(sf::IntRect(col * 320, row * 368, 320, 368));
            animationTimer = 0.0f;
        }
        sprite.setPosition(x, y);
    }
}

void TwinCannons::draw(sf::RenderWindow& window)
{
    if (hasSprite)
        window.draw(sprite);
    else
        window.draw(shape);

    if (leftTurret->isAlive())
        leftTurret->draw(window);
    if (rightTurret->isAlive())
        rightTurret->draw(window);
}

void TwinCannons::shoot()
{
    //Not used - turrets handle their own shooting
}

void TwinCannons::onCollision(GameObject* other)
{
    //Stub
}

void TwinCannons::takeDamage(int damage)
{
    if (coreVulnerable)
    {
        health -= damage;                 //Only damage core if vulnerable
        if (health <= 0)
            alive = false;
    }
    //If not vulnerable, damage is ignored
}

void TwinCannons::loadTurretTexture(sf::Texture& tex)
{
    leftTurret->loadTexture(tex, 0);   //Left turret = even frames (0,2,4,...)
    rightTurret->loadTexture(tex, 1);  //Right turret = odd frames (1,3,5,...)
}

Turret* TwinCannons::getLeftTurret()
{
    return leftTurret;
}

Turret* TwinCannons::getRightTurret()
{
    return rightTurret;
}

bool TwinCannons::isCoreVulnerable()
{
    return coreVulnerable;
}

void TwinCannons::loadTexture(sf::Texture& tex)
{
    sprite.setTexture(tex);
    sprite.setTextureRect(sf::IntRect(2 * 320, 0, 320, 368));  //Row 0, Col 2 (3rd column)
    sprite.setOrigin(160, 184);
    sprite.setScale(0.4f, 0.4f);
    sprite.setPosition(x, y);
    hasSprite = true;
}

