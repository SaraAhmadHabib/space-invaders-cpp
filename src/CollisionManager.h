#pragma once
#include "BulletManager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "PowerUpManager.h"
#include "AsteroidManager.h"
#include "ScoreManager.h"
#include "TwinCannons.h"  

class CollisionManager
{
public:
    //static to call the functions without creating a CollisionManager object
    static void checkBulletVsEnemies(BulletManager* bm, EnemyManager* em, PowerUpManager* pm, ScoreManager* sm);  //Player bullets hit enemies
    static void checkBulletVsPlayer(BulletManager* bm, Player* player, ScoreManager* sm);                 //Enemy bullets hit player
    static void checkEnemyVsPlayer(EnemyManager* em, Player* player, ScoreManager* sm);     //Enemy contact damages player
    static void checkPowerUpVsPlayer(PowerUpManager* pm, Player* player);  //PowerUp collection by player
    static void checkBulletVsAsteroids(BulletManager* bm, AsteroidManager* am);  //Bullets hit asteroids
    static void checkPlayerVsAsteroids(Player* player, AsteroidManager* am, ScoreManager* sm);   //Player hits asteroids
    static void checkBulletVsBoss(BulletManager* bm, Enemy* boss, ScoreManager* sm);  //Player bullets vs boss
    static void checkBulletVsTurrets(BulletManager* bm, TwinCannons* boss);      
};