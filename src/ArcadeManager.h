#pragma once
#include "EnemyManager.h"
#include "BulletManager.h"
#include "Mothership.h"
#include <cstdlib>

class TwinCannons;  //Forward declaration

class ArcadeManager
{
private:
    EnemyManager* enemyManager;  //Stored for Mothership
    BulletManager* bulletManager;
    Enemy* boss;                 //Generic boss pointer — can hold Cruiser or TwinCannons   
    int currentLevel;           //Which level: 1, 2, or 3
    int currentWave;            //Which wave within the level: 1, 2, or 3
    int enemiesInWave;          //Total enemies to spawn in this wave
    int enemiesSpawned;         //How many have been spawned so far
    float spawnTimer;           //Countdown until next spawn
    float spawnInterval;        //Time between each enemy spawn (1 second)
    bool waveActive;            //Is the current wave still spawning?
    bool bossActive;            //Is a boss fight happening?
    bool levelComplete;         //Has the current level been beaten?
    bool gameWon;               //Have all 3 levels been beaten?
    bool levelJustStarted;  //True when a new level begins
    float bossDeathTimer;
    bool bossDying;

    const int MAX_ON_SCREEN = 5;  //Maximum enemies allowed on screen at once

public:
    ArcadeManager(BulletManager* bm, EnemyManager* em);        //Constructor - sets level 1, wave 1

    void update(float deltaTime, EnemyManager* em, BulletManager* bm, float playerX);  //Called every frame - spawns enemies, checks conditions
    bool isWaveComplete(EnemyManager* em);  //Returns true when all enemies in wave are spawned AND killed
    void startWave();           //Begins a new wave - sets enemy count and types based on level/wave
    void startBoss();           //Spawns the boss for current level (stub until Chunks 17-19)
    void nextLevel();           //Advances to next level, resets waves
    void drawBoss(sf::RenderWindow& window);  //Draws the active boss
    void reset();  //Reset to level 1, wave 1
    bool hasLevelStarted();     //Check if level announcement needed
    void clearLevelFlag();      //Reset after announcement
    Enemy* getBoss();                     //Returns current boss                    

    //Getters
    int getCurrentLevel();      //Returns current level number
    int getCurrentWave();       //Returns current wave number
    bool isBossActive();        //Returns true during boss fight
    bool isLevelComplete();     //Returns true when level beaten
    bool isGameWon();           //Returns true when all 3 levels complete
};