#include "ArcadeManager.h"
#include "Drone.h"
#include "Viper.h"
#include "Cruiser.h"
#include "Seeker.h"
#include "TwinCannons.h"
#include <SFML/Audio.hpp>

extern sf::Texture droneTexture;
extern sf::Texture viperTexture;  //Global viper texture from main.cpp
extern sf::Texture seekerTexture;  //Global seeker texture from main.cpp
extern sf::Texture cruiserTexture;
extern sf::Texture twincannonsTexture;
extern sf::Texture mothershipTexture;
extern sf::Texture turretTexture;
extern bool showWaveAnnounce;
extern float waveAnnounceTimer;
extern std::string announcedText;
extern sf::Music menuMusic;
extern sf::Music bossMusic;

ArcadeManager::ArcadeManager(BulletManager* bm, EnemyManager* em)
{
	bulletManager = bm;
	enemyManager = em;
	currentLevel =1;          //Start at level 1
	currentWave = 0;           //Will become 1 when startWave() runs
	enemiesInWave = 0;
	boss = nullptr;
	enemiesSpawned = 0;
	spawnTimer = 0.0f;
	spawnInterval = 1.0f;     //One enemy per second
	waveActive = false;
	bossActive = false;
	levelComplete = false;
	gameWon = false;
	levelJustStarted = false;
	bossDeathTimer = 0.0f;
	bossDying = false;
}

void ArcadeManager::startWave()
{
	currentWave++;                                 //Increment wave (1, 2, or 3)
	enemiesSpawned = 0;                            //Reset spawn counter
	spawnTimer = 0.0f;                             //Reset timer
	waveActive = true;                             //Begin spawning

	//Set enemy count based on level and wave
	if (currentLevel == 1)
		enemiesInWave = 3 + currentWave * 2;        //Level 1: 5, 7, 9
	else if (currentLevel == 2)
		enemiesInWave = 4 + currentWave * 2;        //Level 2: 6, 8, 10
	else
		enemiesInWave = 5 + currentWave * 2;        //Level 3: 7, 9, 11
	if (currentWave == 1)
		levelJustStarted = true;
}

void ArcadeManager::update(float deltaTime, EnemyManager* em, BulletManager* bm, float playerX)
{
	//Update boss if active
	if (bossActive && boss != nullptr)
	{
		boss->update(deltaTime);

		if (!boss->isAlive())
		{
			if (!bossDying)
			{
				bossDying = true;
				bossDeathTimer = 0.5f;  
			}
		}
		if (bossDying)
		{
			bossDeathTimer -= deltaTime;
			if (bossDeathTimer <= 0.0f)
			{
				delete boss;
				boss = nullptr;
				bossActive = false;
				bossDying = false;
				bossMusic.stop();
				menuMusic.play();
				nextLevel();
			}
		}
		return;
	}

	spawnTimer -= deltaTime;  //Countdown to next spawn

	if (spawnTimer <= 0.0f && enemiesSpawned < enemiesInWave)  //Time to spawn
	{
		if (em->getCount() < MAX_ON_SCREEN)  //Only if room on screen
		{
			float randomX = 40.0f + (rand() % 720);  //Random X position

			if (currentLevel == 3)
			{
				int type = rand() % 3;
				if (type == 0)
					em->addEnemy(new Drone(randomX, 65, bm, droneTexture, 1.0f, 1.0f));
				else if (type == 1)
					em->addEnemy(new Viper(randomX, 65, bm, viperTexture, 1.0f, 1.0f));
				else
					em->addEnemy(new Seeker(randomX, 65, playerX, bm, seekerTexture, 1.0f));
			}
			else if (currentLevel >= 2)
			{
				int type = rand() % 2;
				if (type == 0)
					em->addEnemy(new Drone(randomX, 65, bm, droneTexture, 1.0f, 1.0f));
				else
					em->addEnemy(new Viper(randomX, 65, bm, viperTexture, 1.0f, 1.0f));
			}
			else
			{
				em->addEnemy(new Drone(randomX, 65, bm, droneTexture, 1.0f, 1.0f));
			}

			enemiesSpawned++;
		}
		spawnTimer = spawnInterval;  //Reset timer
	}

	//Wave complete?
	if (isWaveComplete(em))
	{
		waveActive = false;

		if (currentWave < 3)
			startWave();      //Next wave
		else
			startBoss();      //Final wave done - spawn boss
	}
}

bool ArcadeManager::isWaveComplete(EnemyManager* em)
{
	return (enemiesSpawned >= enemiesInWave && em->getCount() == 0);  //All spawned AND all dead
}

void ArcadeManager::startBoss()
{
	bossActive = true;
	menuMusic.stop();
	bossMusic.play();

	if (currentLevel == 1)
	{
		Cruiser* c = new Cruiser(400, 80, bulletManager);
		c->loadTexture(cruiserTexture);
		boss = c;
		announcedText = "CRUISER";         //Short name
	}
	else if (currentLevel == 2)
	{
		TwinCannons* tc = new TwinCannons(400, 80, bulletManager);
		tc->loadTexture(twincannonsTexture);
		tc->loadTurretTexture(turretTexture);  //NEW
		boss = tc;
		announcedText = "TWIN CANNONS";    //Short name
	}
	else if (currentLevel == 3)
	{
		Mothership* m = new Mothership(400, 80, bulletManager, enemyManager);
		m->loadTexture(mothershipTexture);
		boss = m;
		announcedText = "MOTHERSHIP";      //Short name
	}

	showWaveAnnounce = true;
	waveAnnounceTimer = 2.0f;
}

void ArcadeManager::nextLevel()
{
	if (currentLevel < 3)
	{
		currentLevel++;      //Advance to next level
		currentWave = 0;     //Reset waves
		bossActive = false;
		startWave();         //Begin first wave of new level
	}
	else
	{
		levelComplete = true;
		gameWon = true;
		bossMusic.stop();
		menuMusic.play();
		delete boss;          //Clean up boss
		boss = nullptr;
		bossActive = false;
	}
}

void ArcadeManager::drawBoss(sf::RenderWindow& window)
{
	if (bossActive && boss != nullptr && !bossDying)
		boss->draw(window);
}

Enemy* ArcadeManager::getBoss()
{
	return boss;
}

int ArcadeManager::getCurrentLevel()
{
	return currentLevel;
}

int ArcadeManager::getCurrentWave()
{
	return currentWave;
}

bool ArcadeManager::isBossActive()
{
	return bossActive;
}

bool ArcadeManager::isLevelComplete()
{
	return levelComplete;
}

bool ArcadeManager::isGameWon()
{
	return gameWon;
}

void ArcadeManager::reset()
{
	currentLevel = 1;
	currentWave = 0;
	enemiesSpawned = 0;
	spawnTimer = 0.0f;
	waveActive = false;
	bossActive = false;
	levelComplete = false;
	levelJustStarted = false;
	gameWon = false;
	if (boss)
	{
		delete boss;
		boss = nullptr;
	}
	bossDeathTimer = 0.0f;
	bossDying = false;
}

bool ArcadeManager::hasLevelStarted()
{
	return levelJustStarted;
}

void ArcadeManager::clearLevelFlag()
{
	levelJustStarted = false;
}