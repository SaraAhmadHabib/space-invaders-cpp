#include "Mothership.h"
#include "Bullet.h"
#include "Seeker.h"
#include <SFML/Audio.hpp>

extern sf::Texture seekerTexture;  //Global seeker texture from main.cpp
extern sf::Texture bulletTexture;
extern sf::Sound sirenSound;

Mothership::Mothership(float startX, float startY, BulletManager* bm, EnemyManager* em)
{
    bulletManager = bm;
    enemyManager = em;
    x = startX;
    y = startY + 65;
    health = 40;                       //Most health of any boss
    maxHealth = 40;
    points = 3000;                     //Biggest reward
    width = 100.0f;                    //Largest boss
    height = 60.0f;
    laserChargeTimer = 5.0f;           //First laser after 5 seconds
    laserCooldown = 5.0f;              //Time between lasers
    laserWarning = false;
    seekerSpawnTimer = 2.0f;           //First seeker after 2 seconds
    seekerSpawnInterval = 3.0f;        //Seeker every 3 seconds
    playerX = 400.0f;                  //Default player position
    vx = 0;
    vy = 0;                            //Doesn't move
    alive = true;

    shape.setFillColor(sf::Color(180, 0, 180));  //Purple
    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(x, y);
    hasSprite = false;
    currentFrame = 0;
    animationTimer = 0.0f;
    warningFlashTimer = 0.0f;
    warningFlashActive = false;
    sirenPlayed = false;
}

void Mothership::update(float deltaTime)
{
    if (!alive) return;  //Stop if dead
    laserChargeTimer -= deltaTime;  //Countdown to next laser attack

    //Warning phase — last 2 seconds before firing (two red rings pulse out)
    if (laserChargeTimer <= 2.0f && laserChargeTimer > 0.0f)
    {
        laserWarning = true;  //Signal main.cpp to draw warning rings
        if (!warningFlashActive)
        {
            warningFlashActive = true;
            warningFlashTimer = 2.0f;

            if (!sirenPlayed)
            {
                sirenSound.play();
                sirenPlayed = true;
            }
        }
    }
    else
    {
        laserWarning = false;         //Stop warning rings
        warningFlashActive = false;   //Reset for next cycle
        sirenPlayed = false;  //Reset for next warning
    }

    if (warningFlashTimer > 0)
        warningFlashTimer -= deltaTime;  //Countdown warning ring timer

    //Fire laser when charge timer expires
    if (laserChargeTimer <= 0.0f)
    {
        fireLaser();                        //Spawn massive laser wall
        laserChargeTimer = laserCooldown;   //Reset charge timer for next attack
    }

    //Seeker spawning
    seekerSpawnTimer -= deltaTime;
    if (seekerSpawnTimer <= 0.0f)
    {
        spawnSeeker();                         //Spawn a seeker enemy
        seekerSpawnTimer = seekerSpawnInterval; //Reset seeker timer
    }

    shape.setPosition(x, y);  //Sync shape position

    //Animate mothership sprite
    if (hasSprite)
    {
        animationTimer += deltaTime;
        if (animationTimer > 0.15f)  //Switch frame every 0.15 seconds
        {
            currentFrame = (currentFrame + 1) % 13;  //Cycle through 13 frames
            sprite.setTextureRect(sf::IntRect(currentFrame * 320, 0, 320, 240));
            animationTimer = 0.0f;
        }
        sprite.setPosition(x, y);  //Sync sprite position
    }
}

void Mothership::fireLaser()
{
    //8 rows for a thick, massive laser wall
    for (int row = 0; row < 20; row++)
    {
        for (float bx = 90.0f; bx < 720.0f; bx += 12.0f)
        {
            Bullet* b = new Bullet(bx, y + height / 2 + row * 10.0f, 0, 400.0f, OWNER_ENEMY);
            b->loadTexture(bulletTexture);
            bulletManager->addBullet(b);
        }
    }
}

void Mothership::spawnSeeker()
{
    float seekerX = x + (rand() % 60 - 30);  //Random offset near mothership X
    Seeker* seeker = new Seeker(seekerX, y + height / 2, playerX, bulletManager, seekerTexture,1.0f);
    enemyManager->addEnemy(seeker);
}

void Mothership::draw(sf::RenderWindow& window)
{
    if (hasSprite)
        window.draw(sprite);
    else
        window.draw(shape);
}

void Mothership::shoot()
{
    //Not used — Mothership uses fireLaser() and spawnSeeker()
}

void Mothership::setPlayerX(float px)
{
    playerX = px;  //Update player position for seeker targeting
}

void Mothership::onCollision(GameObject* other)
{
    //Stub
}

void Mothership::loadTexture(sf::Texture& tex)
{
    sprite.setTexture(tex);
    sprite.setTextureRect(sf::IntRect(0, 0, 320, 240));
    sprite.setOrigin(160, 120);
    sprite.setScale(0.6f, 0.6f);
    sprite.setPosition(x, y);
    hasSprite = true;
}

bool Mothership::isLaserWarning()
{
    return laserWarning;
}

float Mothership:: getWarningTimer() 
{ 
    return warningFlashTimer;
}

Mothership::~Mothership()
{
    extern sf::Sound sirenSound;
    sirenSound.stop();
}