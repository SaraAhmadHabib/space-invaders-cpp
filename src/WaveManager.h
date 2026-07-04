#pragma once
#include "EnemyManager.h"
#include "BulletManager.h"
#include <cstdlib>


class WaveManager
{
	int waveNumber;             //Current wave
	int enemiesInWave;          //Total enemies for this wave
	int enemiesSpawned;         //How many have appeared so far
	float spawnTimer;           //Countdown between spawns
	float spawnInterval;        //Time between each enemy appearing
	bool waveActive;            //Is wave currently running?
	float speedMultiplier;      //Enemy speed scaling
	float fireRateMultiplier;   //Enemy fire rate scaling
	bool justStarted;  //True when a new wave just began

	const int MAX_ON_SCREEN = 5;  //Add this

public:
	WaveManager();              //Constructor, initializes all values

	void startWave();           //Sets up new wave: increments number, calculates enemies, resets counters
	void update(float deltaTime, EnemyManager* em, BulletManager* bm, float playerX);  //Spawns enemies, checks wave complete
	bool isWaveComplete(EnemyManager* em);  //True when all enemies spawned AND all dead
	void reset();  //Reset to wave 1

	int getWaveNumber();                //Returns current wave number
	float getSpeedMultiplier();         //Returns speed scaling for enemies
	float getFireRateMultiplier();      //Returns fire rate scaling for enemies
	bool hasNewWaveStarted();                   //Check if announcement needed
	void clearWaveFlag();                       //Reset after announcement shown
};
