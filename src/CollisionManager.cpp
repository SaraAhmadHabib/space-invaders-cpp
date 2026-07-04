#include "CollisionManager.h"
#include "TwinCannons.h"
#include <SFML/Audio.hpp>
#include <iostream>

extern sf::Texture powerupSpreadTexture;    //Global spread power-up texture
extern sf::Texture powerupPiercingTexture;  //Global piercing power-up texture
extern sf::Texture powerupShieldTexture;    //Global shield power-up texture
extern sf::Texture powerupEMPTexture;       //Global EMP power-up texture
#include "Explosion.h"
extern void addExplosion(Explosion* e);
extern sf::Texture explosionTexture;
extern bool playerWasHit;
extern sf::Sound explosionSound;
extern sf::Sound powerupSound;

void CollisionManager::checkBulletVsEnemies(BulletManager* bm, EnemyManager* em, PowerUpManager* pm, ScoreManager* sm)
{
    for (int b = 0; b < bm->getCount(); b++)  //Loop through all bullets
    {
        Bullet* bullet = bm->getBullet(b);    //Get bullet at index

        if (!bullet->isAlive())               //Skip dead bullets
            continue;

        for (int e = 0; e < em->getCount(); e++)    //Loop through all enemies
        {
            Enemy* enemy = em->getEnemy(e);       //Get enemy at index

            if (!enemy->isAlive())            //Skip dead enemies
                continue;

            if (bullet->getOwner() == OWNER_PLAYER && bullet->getHitbox().intersects(enemy->getHitbox()))  //Check collision
            {
                enemy->takeDamage(bullet->getDamage());  //Damage enemy
                Explosion* exp = new Explosion(enemy->getX(), enemy->getY(), explosionTexture);
                addExplosion(exp);
                explosionSound.setVolume(20.0f);  //Default for small enemies
                explosionSound.play();

                //Power-up drops only on bullet kills
                if (!enemy->isAlive())  //Enemy died from this bullet
                {
                    sm->addKill(enemy->getPoints());  //Add score with multiplier

                    if (rand() % 100 < 15)  //15% weapon/shield drop
                    {
                        int type = rand() % 3;
                        PowerUp* p = new PowerUp(enemy->getX(), enemy->getY(), type);
                        //Load appropriate texture based on type
                        if (type == 0)
                            p->loadTexture(powerupSpreadTexture);    //Spread - yellow
                        else if (type == 1)
                            p->loadTexture(powerupPiercingTexture);  //Piercing - pink
                        else if (type == 2)
                            p->loadTexture(powerupShieldTexture);    //Shield - blue
                        pm->addPowerUp(p);
                    }

                    if (rand() % 100 < 5)  //5% EMP drop
                    {
                        PowerUp* p = new PowerUp(enemy->getX(), enemy->getY(), POWERUP_EMP);
                        p->loadTexture(powerupEMPTexture);  //EMP - purple
                        pm->addPowerUp(p);
                    }
                }

                if (bullet->canPierce() && !bullet->getHasPierced())  //Can pierce and hasn't yet
                {
                    bullet->setHasPierced(true);  //Mark as pierced - bullet survives this hit
                }
                else
                {
                    bullet->setAlive(false);  //No pierce or already pierced - destroy bullet
                }

                break;  //Exit enemy loop                                
            }
        }
    }
}


void CollisionManager::checkBulletVsPlayer(BulletManager* bm, Player* player, ScoreManager* sm)
{
	for (int b = 0; b < bm->getCount(); b++)  //Loop through all bullets
	{
		Bullet* bullet = bm->getBullet(b);    //Get bullet at index

		if (!bullet->isAlive())               //Skip dead bullets
			continue;

		if (bullet->getOwner() == OWNER_ENEMY && bullet->getHitbox().intersects(player->getHitbox()))  //Check collision
		{
			player->takeDamage(bullet->getDamage());  //Damage player
            playerWasHit = true;
            sm->resetMultiplier();  //Reset multiplier on hit
			bullet->setAlive(false);                 //Destroy bullet
		}
	}
}

void CollisionManager::checkEnemyVsPlayer(EnemyManager* em, Player* player, ScoreManager* sm)
{
	for (int e = 0; e < em->getCount(); e++)    //Loop through all enemies
	{
		Enemy* enemy = em->getEnemy(e);    //Get enemy at index

		if (!enemy->isAlive())               //Skip dead enemies
			continue;

		if (enemy->getHitbox().intersects(player->getHitbox()))  //Check collision
		{
			player->takeDamage(1);  //Damage player, When bosses arrive, we add contactDamage variable
            playerWasHit = true;
            sm->resetMultiplier();  //Reset multiplier on hit
			enemy->setAlive(false);  //Destroy enemy
		}
	}
}

void CollisionManager::checkPowerUpVsPlayer(PowerUpManager* pm, Player* player)
{
	for (int p = 0; p < pm->getCount(); p++)  //Loop through all power-ups
	{
		PowerUp* powerUp = pm->getPowerUp(p);  //Get power-up at index

		if (!powerUp->isAlive())               //Skip dead power-ups
			continue;

		if (powerUp->getHitbox().intersects(player->getHitbox()))  //Check if player touches power-up
		{
			powerUp->applyEffect(player);      //Apply power-up effect to player
            powerupSound.play();
			powerUp->setAlive(false);          //Remove power-up
		}
	}
}

void CollisionManager::checkBulletVsAsteroids(BulletManager* bm, AsteroidManager* am)
{
    for (int b = 0; b < bm->getCount(); b++)  //Loop through all bullets
    {
        Bullet* bullet = bm->getBullet(b);    //Get bullet at index

        if (!bullet->isAlive())               //Skip dead bullets
            continue;

        for (int a = 0; a < am->getCount(); a++)  //Loop through all asteroids
        {
            Asteroid* asteroid = am->getAsteroid(a);  //Get asteroid at index

            if (!asteroid->isAlive())                //Skip dead asteroids
                continue;

            if (bullet->getHitbox().intersects(asteroid->getHitbox()))  //Bullet hits asteroid
            {
                bullet->setAlive(false);  //Destroy bullet
                break;                    //Exit asteroid loop
            }
        }
    }
}

void CollisionManager::checkPlayerVsAsteroids(Player* player, AsteroidManager* am, ScoreManager* sm)
{
    for (int a = 0; a < am->getCount(); a++)  //Loop through all asteroids
    {
        Asteroid* asteroid = am->getAsteroid(a);  //Get asteroid at index

        if (!asteroid->isAlive())                //Skip dead asteroids
            continue;

        if (player->getHitbox().intersects(asteroid->getHitbox()))  //Player hits asteroid
        {
            player->takeDamage(1);  //Player takes damage
            playerWasHit = true;
            sm->resetMultiplier();  //Reset multiplier on hit

            //Push player away so asteroid feels solid
            if (player->getX() < asteroid->getX())
                player->setPosition(player->getX() - 20, player->getY());  //Push left
            else
                player->setPosition(player->getX() + 20, player->getY());  //Push right
        }
    }
}

void CollisionManager::checkBulletVsBoss(BulletManager* bm, Enemy* boss, ScoreManager* sm)
{
    if (boss == nullptr || !boss->isAlive())
        return;

    for (int b = 0; b < bm->getCount(); b++)
    {
        Bullet* bullet = bm->getBullet(b);

        if (!bullet->isAlive())
            continue;

        if (bullet->getOwner() == OWNER_PLAYER && bullet->getHitbox().intersects(boss->getHitbox()))
        {
            TwinCannons* tc = dynamic_cast<TwinCannons*>(boss);
            if (tc && !tc->isCoreVulnerable())
            {
                bullet->setAlive(false);
                continue;
            }

            boss->takeDamage(bullet->getDamage());  //THIS WAS MISSING

            std::cout << "Boss health: " << boss->getHealth() << "/" << boss->getMaxHealth() << std::endl;
            bullet->setAlive(false);

            //Boss just died - spawn big explosions
            //Boss just died - spawn big explosions
            if (!boss->isAlive())
            {
                float bx = boss->getX();
                float by = boss->getY();

                int count = 8 + (rand() % 3);  //8 to 10 explosions
                for (int i = 0; i < count; i++)
                {
                    float offsetX = (rand() % 200) - 100;  //Spread 100px left/right
                    float offsetY = (rand() % 160) - 80;   //Spread 80px up/down

                    Explosion* exp = new Explosion(bx + offsetX, by + offsetY, explosionTexture);
                    exp->setScale(0.6f, 0.6f);
                    exp->setFrameTimer((rand() % 20) * 0.02f);
                    addExplosion(exp);
                    explosionSound.setVolume(60.0f);  //Louder for boss
                    explosionSound.play();
                    explosionSound.setVolume(20.0f);  //Reset for small enemies
                }
            }
        }
    }
}

void CollisionManager::checkBulletVsTurrets(BulletManager* bm, TwinCannons* boss)
{
    if (boss == nullptr || !boss->isAlive())  //No boss or boss dead
        return;

    Turret* left = boss->getLeftTurret();
    Turret* right = boss->getRightTurret();

    for (int b = 0; b < bm->getCount(); b++)  //Loop through all bullets
    {
        Bullet* bullet = bm->getBullet(b);    //Get bullet at index

        if (!bullet->isAlive())               //Skip dead bullets
            continue;

        if (bullet->getOwner() != OWNER_PLAYER)  //Only player bullets
            continue;

        //Check left turret
        if (left && left->isAlive() && bullet->getHitbox().intersects(left->getHitbox()))
        {
            left->takeDamage(bullet->getDamage());
            bullet->setAlive(false);

            if (!left->isAlive())  //Turret just died
            {
                Explosion* exp = new Explosion(left->getX(), left->getY() + 55, explosionTexture);
                exp->setScale(0.4f, 0.4f);
                addExplosion(exp);
                explosionSound.setVolume(60.0f);
                explosionSound.play();
                explosionSound.setVolume(20.0f);
            }

            std::cout << "Left turret health: " << left->getHealth() << "/" << left->getMaxHealth() << std::endl;
            continue;
        }

        //Check right turret
        if (right && right->isAlive() && bullet->getHitbox().intersects(right->getHitbox()))
        {
            right->takeDamage(bullet->getDamage());
            bullet->setAlive(false);

            if (!right->isAlive())  //Turret just died
            {
                Explosion* exp = new Explosion(right->getX(), right->getY() + 55, explosionTexture);
                exp->setScale(0.4f, 0.4f);
                addExplosion(exp);
                explosionSound.setVolume(60.0f);
                explosionSound.play();
                explosionSound.setVolume(20.0f);
            }

            std::cout << "Right turret health: " << right->getHealth() << "/" << right->getMaxHealth() << std::endl;
        }
    }
}