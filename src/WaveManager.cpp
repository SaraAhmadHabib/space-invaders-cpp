#include "WaveManager.h"
#include "Drone.h"
#include "Seeker.h"
#include "Viper.h"

extern sf::Texture droneTexture;
extern sf::Texture viperTexture;  //Global viper texture from main.cpp
extern sf::Texture seekerTexture;  //Global seeker texture from main.cpp

WaveManager::WaveManager()
{
    waveNumber = 0;
	enemiesInWave = 5;
	enemiesSpawned = 0;
	spawnTimer = 0.0f;
	spawnInterval = 1.0f;
	waveActive = false;
	speedMultiplier = 1.0f;
	fireRateMultiplier = 1.0f;
    justStarted = false;
}

void WaveManager::startWave()
{
	waveNumber++;                                    //Increment wave
	enemiesSpawned = 0;                              //Reset spawn counter
	enemiesInWave = 5 + (waveNumber - 1) * 2;        //5, 7, 9, 11...

	speedMultiplier = 1.0f + (waveNumber - 1) * 0.05f;       //+5% per wave
	fireRateMultiplier = 1.0f + (waveNumber - 1) * 0.10f;    //+10% per wave

	waveActive = true;                               //Begin spawning
    justStarted = true;
}

void WaveManager::update(float deltaTime, EnemyManager* em, BulletManager* bm, float playerX)
{
    if (!waveActive)
        return;

    spawnTimer -= deltaTime;  //Countdown to next spawn

    if (spawnTimer <= 0.0f && enemiesSpawned < enemiesInWave)  //Time to spawn and wave not done
    {
        if (em->getCount() < MAX_ON_SCREEN)  //Only spawn if room on screen
        {
            float randomX = 30.0f + (rand() % 740);
            float sm = speedMultiplier;        //ADD THIS
            float fm = fireRateMultiplier;     //ADD THIS

            if (waveNumber >= 8)
            {
                int type = rand() % 3;
                if (type == 0)
                    em->addEnemy(new Drone(randomX, 65, bm, droneTexture, sm, fm));
                else if (type == 1)
                    em->addEnemy(new Viper(randomX, 65, bm, viperTexture, sm, fm));
                else
                    em->addEnemy(new Seeker(randomX, 65, playerX, bm, seekerTexture, sm));
            }
            else if (waveNumber >= 5)
            {
                int type = rand() % 2;
                if (type == 0)
                    em->addEnemy(new Drone(randomX, 65, bm, droneTexture, sm, fm));
                else
                    em->addEnemy(new Viper(randomX, 65, bm, viperTexture, sm, fm));
            }
            else
            {
                em->addEnemy(new Drone(randomX, 65, bm, droneTexture, sm, fm));
            }

            enemiesSpawned++;  //Count this spawn
        }
        spawnTimer = spawnInterval;  //Reset spawn timer
    }

    if (isWaveComplete(em))  //All enemies spawned and killed
    {
        waveActive = false;
        startWave();  //Begin next wave
    }
}

bool WaveManager::isWaveComplete(EnemyManager* em)
{
    return (enemiesSpawned >= enemiesInWave && em->getCount() == 0);  //All spawned AND all dead
}

int WaveManager::getWaveNumber()
{
    return waveNumber;  //Return current wave
}

float WaveManager::getSpeedMultiplier()
{
    return speedMultiplier;  //Return speed scaling
}

float WaveManager::getFireRateMultiplier()
{
    return fireRateMultiplier;  //Return fire rate scaling
}

void WaveManager::reset()
{
    waveNumber = 0;
    enemiesSpawned = 0;
    enemiesInWave = 5;
    spawnTimer = 0.0f;
    waveActive = false;
    speedMultiplier = 1.0f;
    fireRateMultiplier = 1.0f;
}

bool WaveManager::hasNewWaveStarted()
{
    return justStarted;  //Check if announcement needed
}

void WaveManager::clearWaveFlag()
{
    justStarted = false;  //Reset after announcement shown
}