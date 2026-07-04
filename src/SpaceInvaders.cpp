#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "BulletManager.h"
#include "CollisionManager.h"
#include "PowerUpManager.h"
#include "EnemyManager.h"
#include "GameStateManager.h"
#include "Mothership.h"
#include "Starfield.h"
#include "ScoreManager.h"
#include "TwinCannons.h"
#include "AsteroidManager.h"
#include "ArcadeManager.h"
#include "Explosion.h"
#include "WaveManager.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;

sf::Texture droneTexture;
sf::Texture viperTexture;  //Global viper sprite texture
sf::Texture seekerTexture;  //Global seeker sprite texture
sf::Texture powerupSpreadTexture;   //Global spread power-up texture
sf::Texture powerupPiercingTexture; //Global piercing power-up texture
sf::Texture powerupShieldTexture;   //Global shield power-up texture
sf::Texture powerupEMPTexture;      //Global EMP power-up texture
sf::Texture asteroidTexture;  //Global asteroid spritesheet texture
sf::Texture bulletTexture;  //Global bullet spritesheet texture
sf::Texture cruiserTexture;  //Global cruiser texture
sf::Texture twincannonsTexture;
sf::Texture explosionTexture;
sf::Texture mothershipTexture;
sf::Texture turretTexture;
sf::Sound shootSound;
sf::SoundBuffer shootBuffer;
sf::Sound explosionSound;
sf::SoundBuffer explosionBuffer;
sf::Sound hitSound;
sf::SoundBuffer hitBuffer;
sf::Sound powerupSound;
sf::SoundBuffer powerupBuffer;
sf::SoundBuffer shieldBreakBuffer;
sf::Sound shieldBreakSound;
sf::SoundBuffer empBuffer;
sf::Sound empSound;
sf::SoundBuffer winCheerBuffer;
sf::Sound winCheerSound;
sf::Music gameoverMusic;
sf::Music bossMusic;
sf::Music menuMusic;
sf::SoundBuffer sirenBuffer;
sf::Sound sirenSound;
sf::Texture shieldBreakTexture;

bool showWaveAnnounce = false;       //Show wave announcement?
float waveAnnounceTimer = 0.0f;      //How long to show
int announcedWave = 0;               //Which wave is being announced
std::string announcedText = "";  //What text to show during announcement
sf::Texture trophyTexture;

Explosion** explosions = nullptr;
int explosionCount = 0;
int explosionCapacity = 0;

bool playerWasHit = false;
bool bossEMPFlash = false;
float bossEMPFlashTimer = 0.0f;

int highScore = 0;

void addExplosion(Explosion* e)
{
	if (explosionCount == explosionCapacity)
	{
		int newCap = explosionCapacity + 5;
		Explosion** newArr = new Explosion * [newCap];
		for (int i = 0; i < explosionCount; i++)
			newArr[i] = explosions[i];
		delete[] explosions;
		explosions = newArr;
		explosionCapacity = newCap;
	}
	explosions[explosionCount++] = e;
}

void saveHighScore(int score)
{
	if (score > highScore)
	{
		highScore = score;
		ofstream scoreFile("highscore.txt");
		if (scoreFile.is_open())
		{
			scoreFile << highScore;
			scoreFile.close();
		}
	}
}

int main()
{
	srand(time(0));  //Seed random

	ifstream scoreFile("highscore.txt");
	if (scoreFile.is_open())
	{
		scoreFile >> highScore;
		scoreFile.close();
	}

	//Dash cooldown bar
	sf::RectangleShape dashBarBg;     //Background (empty)
	sf::RectangleShape dashBarFill;   //Fill (depletes/refills)

	sf::Texture invincibilityTexture;
	invincibilityTexture.loadFromFile("assets/orbs.png");

	sf::Texture shieldTexture;
	shieldTexture.loadFromFile("assets/shield.png");

	//Managers
	BulletManager bulletManager;
	EnemyManager enemyManager;
	PowerUpManager powerUpManager;
	AsteroidManager asteroidManager;
	ScoreManager scoreManager;
	WaveManager waveManager;
	Starfield starfield;
	ArcadeManager arcadeManager(&bulletManager, &enemyManager);
	GameStateManager gameState;
	int selectedOption = 0;     //0 = Arcade, 1 = Survival
	int gameMode = 0;           //Set when player selects from menu
	bool screenShake = false;
	float shakeTimer = 0.0f;
	float shakeDuration = 0.3f;  //How long shake lasts
	float shakeIntensity = 8.0f;  //How much it shakes
	extern bool empFlash;
	float empFlashTimer = 0.0f;
	int displayedScore = 0;           //Score shown on screen (counts up)
	float scoreCountTimer = 0.0f;     //Timer for counting animation
	bool scoreCounting = false;       //True while counting up
	std::string bossName = "";
	bool winCountTriggered = false;  //Only trigger win count once

	//Player
	sf::Texture playerTexture;
	playerTexture.loadFromFile("assets/player.png");

	//Drone
	droneTexture.loadFromFile("assets/drone.png");

	//Viper
	viperTexture.loadFromFile("assets/viper.png");  //Load viper sprite image

	//Seeker
	seekerTexture.loadFromFile("assets/seeker.png");  //Load seeker sprite image

	powerupSpreadTexture.loadFromFile("assets/powerup_spread.png");      //Load spread sprite
	powerupPiercingTexture.loadFromFile("assets/powerup_piercing.png");  //Load piercing sprite
	powerupShieldTexture.loadFromFile("assets/powerup_shield.png");      //Load shield sprite
	powerupEMPTexture.loadFromFile("assets/powerup_emp.png");            //Load EMP sprite
	bulletTexture.loadFromFile("assets/bullets.png");  //Load bullet spritesheet
	explosionTexture.loadFromFile("assets/explosion.png");
	cruiserTexture.loadFromFile("assets/cruiser.png");
	twincannonsTexture.loadFromFile("assets/twincannons.png");
	mothershipTexture.loadFromFile("assets/mothership.png");
	trophyTexture.loadFromFile("assets/trophy.png");
	turretTexture.loadFromFile("assets/Turrets.png");
	shieldBreakTexture.loadFromFile("assets/shield_break.png");

	//Window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Space Invaders");

	//Player
	Player player(&bulletManager, &enemyManager, &arcadeManager);
	player.loadTexture(playerTexture);  //Load player sprite
	player.loadShieldBreakTexture(shieldBreakTexture);
	player.loadInvincibilityOrb(invincibilityTexture);
	player.loadShieldOrbTexture(shieldTexture);  //Different file
	sf::Texture playerSpreadTexture;
	playerSpreadTexture.loadFromFile("assets/Player_Spread.png");

	sf::Texture playerPiercingTexture;
	playerPiercingTexture.loadFromFile("assets/Player_Piercing.png");

	player.setSpreadTexture(playerSpreadTexture);
	player.setPiercingTexture(playerPiercingTexture);

	//Audio
	menuMusic.openFromFile("assets/menu_bgm.wav");
	menuMusic.setLoop(true);
	sf::Music::TimeSpan loopSpan(sf::seconds(0), sf::seconds(35));
	menuMusic.setLoopPoints(loopSpan);
	menuMusic.play();
	shootBuffer.loadFromFile("assets/playershoot.wav");
	shootSound.setBuffer(shootBuffer);
	shootSound.setVolume(40.0f);  //40% volume (default is 100)

	explosionBuffer.loadFromFile("assets/explosion.wav");
	explosionSound.setBuffer(explosionBuffer);
	explosionSound.setVolume(40.0f);  

	hitBuffer.loadFromFile("assets/hit.wav");
	hitSound.setBuffer(hitBuffer);
	hitSound.setVolume(100.0f);
	
	powerupBuffer.loadFromFile("assets/powerup.wav");
	powerupSound.setBuffer(powerupBuffer);
	powerupSound.setVolume(50.0f);

	shieldBreakBuffer.loadFromFile("assets/shield_break.wav");
	shieldBreakSound.setBuffer(shieldBreakBuffer);
	shieldBreakSound.setVolume(80.0f);


	empBuffer.loadFromFile("assets/emp.wav");
	empSound.setBuffer(empBuffer);
	empSound.setVolume(100.0f);

	winCheerBuffer.loadFromFile("assets/win_cheer.wav");
	winCheerSound.setBuffer(winCheerBuffer);
	winCheerSound.setVolume(80.0f);

	gameoverMusic.openFromFile("assets/gameover_music.wav");
	gameoverMusic.setVolume(40.0f);  //ADD THIS
	gameoverMusic.setLoop(false);

	bossMusic.openFromFile("assets/boss_bgm.wav");
	bossMusic.setVolume(40.0f);  //40% volume
	bossMusic.setLoop(true);

	sirenBuffer.loadFromFile("assets/siren.wav");
	sirenSound.setBuffer(sirenBuffer);
	sirenSound.setVolume(50.0f);

	//Clock
	sf::Clock clock;

	//Font
	sf::Font font;
	font.loadFromFile("assets/font.ttf");

	//Planet texture
	sf::Texture planetTexture;
	planetTexture.loadFromFile("assets/planets.png");

	//star Texture
	sf::Texture starTexture;
	starTexture.loadFromFile("assets/star.png");

	//Heart Texture
	sf::Texture heartTexture;
	heartTexture.loadFromFile("assets/heart.png");

	//HUD - Row 1 (top)
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(12);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(20, 6);

	sf::Text weaponText;
	weaponText.setFont(font);
	weaponText.setCharacterSize(12);
	weaponText.setFillColor(sf::Color::White);
	weaponText.setPosition(290, 6);

	sf::Text shieldText;
	shieldText.setFont(font);
	shieldText.setCharacterSize(12);
	shieldText.setFillColor(sf::Color::White);
	shieldText.setPosition(650, 6);

	//HUD - Row 2
	sf::Text livesLabel;
	livesLabel.setFont(font);
	livesLabel.setCharacterSize(10);
	livesLabel.setFillColor(sf::Color::White);
	livesLabel.setString("LIVES: ");
	livesLabel.setPosition(20, 24);

	sf::Text multiplierText;
	multiplierText.setFont(font);
	multiplierText.setCharacterSize(14);
	multiplierText.setFillColor(sf::Color::Yellow);
	multiplierText.setPosition(20, 40);

	sf::Text waveText;
	waveText.setFont(font);
	waveText.setCharacterSize(12);
	waveText.setFillColor(sf::Color::White);
	waveText.setPosition(290, 24);

	sf::Text empText;
	empText.setFont(font);
	empText.setCharacterSize(12);
	empText.setFillColor(sf::Color::White);
	empText.setPosition(650, 24);

	//Game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				//Menu navigation
				if (gameState.isMenu())
				{
					if (event.key.code == sf::Keyboard::Up)
						selectedOption = 0;                    //Survival
					if (event.key.code == sf::Keyboard::Down)
						selectedOption = 1;                   //Arcade
					if (event.key.code == sf::Keyboard::Enter)
					{
						gameMode = selectedOption;
						gameState.setState(PLAYING);
						player.reset();
						enemyManager.clearAll();
						bulletManager.clearAll();
						powerUpManager.clearAll();
						asteroidManager.clearAll();
						scoreManager.reset();
						if (gameMode == 0)
						{
							waveManager.reset();
							waveManager.startWave();
						}
						else
						{
							arcadeManager.reset();
							arcadeManager.startWave();
						}
					}
				}
				//Pause toggle
				else if (event.key.code == sf::Keyboard::Escape)
				{
					if (gameState.isPlaying())
						gameState.setState(PAUSED);
					else if (gameState.isPaused())
						gameState.setState(PLAYING);
				}
				else if (gameState.isPaused() && event.key.code == sf::Keyboard::Q)
				{
					gameState.setState(MENU);
					bossMusic.stop();
					sirenSound.stop();  //ADD THIS
					menuMusic.play();  //Restart menu music
					player.reset();
					enemyManager.clearAll();
					bulletManager.clearAll();
					powerUpManager.clearAll();
					asteroidManager.clearAll();
					scoreManager.reset();
				}
				//Game over → return to menu
				else if (gameState.isGameOver() && event.key.code == sf::Keyboard::Enter)
				{
					gameState.setState(MENU);
					gameoverMusic.stop();
					bossMusic.stop();
					menuMusic.play();  //Restart menu music
					saveHighScore(scoreManager.getScore());
					selectedOption = 0;
					gameMode = 0;
					winCountTriggered = false;
					scoreCounting = false;
					displayedScore = 0;
					showWaveAnnounce = false;  //ADD THIS
					empFlashTimer = 0.0f;      //ADD THIS
				}

				//YOU WIN → return to menu
				else if (gameMode == 1 && arcadeManager.isGameWon() && event.key.code == sf::Keyboard::Enter)
				{
					gameState.setState(MENU);
					winCheerSound.stop();  //ADD THIS
					menuMusic.play();
					saveHighScore(scoreManager.getScore());
					selectedOption = 0;
					winCountTriggered = false;
					gameMode = 0;
					scoreCounting = false;
					displayedScore = 0;
				}
			}
		}
		float deltaTime = clock.restart().asSeconds();
		if (!gameState.isPaused())
			starfield.update(deltaTime); //Only scroll when not paused

		if (gameState.isPlaying() && !(gameMode == 1 && arcadeManager.isGameWon()))
		{

			//Update wave system (spawns enemies, handles transitions)
			if (gameMode == 0)
			{
				waveManager.update(deltaTime, &enemyManager, &bulletManager, player.getX());

				if (waveManager.hasNewWaveStarted())
				{
					int w = waveManager.getWaveNumber();
					announcedWave = w;
					announcedText = "WAVE " + std::to_string(w);

					showWaveAnnounce = true;
					waveAnnounceTimer = 1.5f;
					waveManager.clearWaveFlag();
				}
			}
			else
			{
				arcadeManager.update(deltaTime, &enemyManager, &bulletManager, player.getX());

				//Arcade level announcement
				if (arcadeManager.hasLevelStarted() && !arcadeManager.isBossActive())
				{
					announcedText = "LEVEL " + std::to_string(arcadeManager.getCurrentLevel());
					announcedWave = 0;
					showWaveAnnounce = true;
					waveAnnounceTimer = 1.5f;
					arcadeManager.clearLevelFlag();
				}
			}

			//Update player
			player.update(deltaTime);

			//Update systems
			bulletManager.updateAll(deltaTime);
			powerUpManager.updateAll(deltaTime);
			asteroidManager.updateAll(deltaTime);
			//explosion updates
			for (int i = 0; i < explosionCount; i++)
				explosions[i]->update(deltaTime);

			for (int i = explosionCount - 1; i >= 0; i--)
			{
				if (explosions[i]->isFinished())
				{
					delete explosions[i];
					for (int j = i; j < explosionCount - 1; j++)
						explosions[j] = explosions[j + 1];
					explosionCount--;
				}
			}

			//Collisions
			CollisionManager::checkBulletVsEnemies(&bulletManager, &enemyManager, &powerUpManager, &scoreManager);
			CollisionManager::checkBulletVsPlayer(&bulletManager, &player, &scoreManager);
			CollisionManager::checkEnemyVsPlayer(&enemyManager, &player, &scoreManager);
			CollisionManager::checkPowerUpVsPlayer(&powerUpManager, &player);
			CollisionManager::checkBulletVsAsteroids(&bulletManager, &asteroidManager);
			CollisionManager::checkPlayerVsAsteroids(&player, &asteroidManager, &scoreManager);
			//Boss collision (Arcade mode)
			if (gameMode == 1 && arcadeManager.isBossActive())
				CollisionManager::checkBulletVsBoss(&bulletManager, arcadeManager.getBoss(), &scoreManager);
			//Turret collision (Arcade Level 2)
			if (gameMode == 1 && arcadeManager.isBossActive() && arcadeManager.getCurrentLevel() == 2)
			{
				TwinCannons* tc = dynamic_cast<TwinCannons*>(arcadeManager.getBoss());
				if (tc)
					CollisionManager::checkBulletVsTurrets(&bulletManager, tc);
			}

			//Update Mothership player tracking (Arcade Level 3)
			if (gameMode == 1 && arcadeManager.isBossActive() && arcadeManager.getCurrentLevel() == 3)
			{
				Mothership* m = dynamic_cast<Mothership*>(arcadeManager.getBoss());
				if (m)
					m->setPlayerX(player.getX());
			}

			//Update enemies AFTER collision (drops work correctly)
			enemyManager.updateAll(deltaTime);

			//Update score (chain timer)
			scoreManager.update(deltaTime);

			if (player.getLives() <= 0)
			{
				gameState.setState(GAME_OVER);
				bossMusic.stop();
				menuMusic.stop();
				sirenSound.stop();  //ADD THIS
				gameoverMusic.play();
				displayedScore = 0;
				scoreCountTimer = 0.0f;
				scoreCounting = true;
			}
		}

		if (gameMode == 1 && arcadeManager.isGameWon() && !winCountTriggered)
		{
			winCountTriggered = true;
			displayedScore = 0;
			scoreCountTimer = 0.0f;
			scoreCounting = true;
		}

		//Wave announcement timer
		if (showWaveAnnounce)
		{
			waveAnnounceTimer -= deltaTime;
			if (waveAnnounceTimer <= 0)
				showWaveAnnounce = false;
		}

		//Screen shake trigger
		if (playerWasHit)
		{
			screenShake = true;
			shakeTimer = shakeDuration;
			playerWasHit = false;
		}
		if (empFlash)
		{
			empFlashTimer = 0.3f;
			empFlash = false;
			bulletManager.clearAll();
			enemyManager.clearAll();
			if (gameMode == 1 && arcadeManager.isBossActive())
			{
				bossEMPFlashTimer = 0.6f;  //Start countdown
				bossEMPFlash = false;       //Don't flash yet
			}
		}

		if (empFlashTimer > 0)
			empFlashTimer -= deltaTime;

		if (bossEMPFlashTimer > 0)
		{
			bossEMPFlashTimer -= deltaTime;
			if (bossEMPFlashTimer <= 1.0f)  //Flash only in last 0.3 seconds
				bossEMPFlash = true;
		}
		else
			bossEMPFlash = false;

		//Screen shake timer
		if (screenShake)
		{
			shakeTimer -= deltaTime;
			if (shakeTimer <= 0)
				screenShake = false;
		}

		sf::View view = window.getDefaultView();
		if (screenShake)
		{
			float offsetX = (rand() % (int)(shakeIntensity * 2)) - shakeIntensity;
			float offsetY = (rand() % (int)(shakeIntensity * 2)) - shakeIntensity;
			view.move(offsetX, offsetY);
		}
		window.setView(view);

		if (scoreCounting)
		{
			scoreCountTimer += deltaTime;
			int targetScore = scoreManager.getScore();

			//Speed up counting
			displayedScore = (int)(targetScore * (scoreCountTimer / 1.5f));

			if (displayedScore >= targetScore)
			{
				displayedScore = targetScore;
				scoreCounting = false;
			}
		}

		//Draw
		window.clear(sf::Color::Black);

		//EMP shockwave
		if (empFlashTimer > 0)
		{
			float progress = 1.0f - (empFlashTimer / 0.3f);  //0 to 1
			float radius = progress * 700.0f;                  //Grows to 650px
			float alpha = (1.0f - progress) * 200;             //Fades from 200 to 0

			sf::CircleShape shockwave(radius);
			shockwave.setOrigin(radius, radius);
			shockwave.setPosition(400, 300);                    //Center of screen
			shockwave.setFillColor(sf::Color::Transparent);
			shockwave.setOutlineColor(sf::Color(255, 255, 255, (int)alpha));
			shockwave.setOutlineThickness(8.0f - progress * 6.0f);  //Thick outline that thins
			window.draw(shockwave);

			//Inner flash
			sf::CircleShape innerFlash(radius * 0.7f);
			innerFlash.setOrigin(radius * 0.7f, radius * 0.7f);
			innerFlash.setPosition(400, 300);
			innerFlash.setFillColor(sf::Color(200, 220, 255, (int)(alpha * 0.5f)));
			window.draw(innerFlash);
		}
		starfield.draw(window);

		//MENU SCREEN
		if (gameState.isMenu())
		{
			//Decorative stars - evenly scattered, no naked areas
			sf::Sprite star1(starTexture);
			star1.setScale(0.05f, 0.05f);
			star1.setPosition(170, 45);
			window.draw(star1);

			sf::Sprite star2(starTexture);
			star2.setScale(0.04f, 0.04f);
			star2.setPosition(520, 35);
			window.draw(star2);

			sf::Sprite star3(starTexture);
			star3.setScale(0.06f, 0.06f);
			star3.setPosition(730, 130);
			window.draw(star3);

			sf::Sprite star4(starTexture);
			star4.setScale(0.04f, 0.04f);
			star4.setPosition(60, 180);
			window.draw(star4);

			sf::Sprite star5(starTexture);
			star5.setScale(0.05f, 0.05f);
			star5.setPosition(700, 300);
			window.draw(star5);

			sf::Sprite star6(starTexture);
			star6.setScale(0.04f, 0.04f);
			star6.setPosition(90, 350);
			window.draw(star6);

			sf::Sprite star7(starTexture);
			star7.setScale(0.06f, 0.06f);
			star7.setPosition(480, 400);      //Lower - below instructions
			window.draw(star7);

			sf::Sprite star8(starTexture);
			star8.setScale(0.04f, 0.04f);
			star8.setPosition(340, 430);      //Higher - near bottom edge
			window.draw(star8);

			sf::Sprite star9(starTexture);
			star9.setScale(0.05f, 0.05f);
			star9.setPosition(730, 440);
			window.draw(star9);

			//Planet 1 - top left, pulled in from corner
			sf::Sprite planet1(planetTexture);
			planet1.setTextureRect(sf::IntRect(0, 0, 166, 166));       //Row 1, Col 1
			planet1.setScale(0.6f, 0.6f);
			planet1.setPosition(60, 70);
			window.draw(planet1);

			//Planet 2 - top right, pulled in
			sf::Sprite planet2(planetTexture);
			planet2.setTextureRect(sf::IntRect(166, 0, 166, 166));      //Row 1, Col 2
			planet2.setScale(0.5f, 0.5f);
			planet2.setPosition(620, 80);
			window.draw(planet2);

			//Planet 3 - bottom left, different frame to avoid the bad chunk
			sf::Sprite planet3(planetTexture);
			planet3.setTextureRect(sf::IntRect(332, 0, 166, 166));      //Row 1, Col 3 (different planet)
			planet3.setScale(0.45f, 0.45f);
			planet3.setPosition(50, 390);
			window.draw(planet3);

			//Planet 4 - bottom right
			sf::Sprite planet4(planetTexture);
			planet4.setTextureRect(sf::IntRect(166, 332, 166, 166));      //Row 3, Col 2
			planet4.setScale(0.5f, 0.5f);
			planet4.setPosition(610, 380);
			window.draw(planet4);


			//Menu options box
			sf::RectangleShape menuBox(sf::Vector2f(300, 100));
			menuBox.setFillColor(sf::Color(20, 20, 40, 180));
			menuBox.setOutlineColor(sf::Color(100, 100, 200, 80));
			menuBox.setOutlineThickness(2);
			menuBox.setOrigin(150, 50);
			menuBox.setPosition(400, 305);
			window.draw(menuBox);

			sf::Text highScoreGlow("HIGH SCORE: " + std::to_string(highScore), font, 12);
			sf::FloatRect hsBounds = highScoreGlow.getLocalBounds();
			highScoreGlow.setOrigin(hsBounds.width / 2, hsBounds.height / 2);
			highScoreGlow.setPosition(400, 515);
			highScoreGlow.setFillColor(sf::Color(255, 215, 0, 50));  //Gold glow
			window.draw(highScoreGlow);

			sf::Text highScoreText("HIGH SCORE: " + std::to_string(highScore), font, 12);
			highScoreText.setOrigin(hsBounds.width / 2, hsBounds.height / 2);
			highScoreText.setPosition(400, 515);
			highScoreText.setFillColor(sf::Color(255, 215, 0));  //Gold
			window.draw(highScoreText);

			//Title bounds for centering
			sf::Text title("SPACE INVADERS", font, 28);
			sf::FloatRect titleBounds = title.getLocalBounds();

			//Title glow layers
			sf::Text titleGlow1("SPACE INVADERS", font, 28);
			titleGlow1.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
			titleGlow1.setPosition(400, 100);
			titleGlow1.setFillColor(sf::Color(255, 50, 50, 40));
			window.draw(titleGlow1);

			sf::Text titleGlow2("SPACE INVADERS", font, 28);
			titleGlow2.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
			titleGlow2.setPosition(400, 100);
			titleGlow2.setFillColor(sf::Color(255, 80, 80, 80));
			window.draw(titleGlow2);

			//Main title
			title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
			title.setPosition(400, 100);
			title.setFillColor(sf::Color(255, 50, 50));
			window.draw(title);

			//Subtitle
			sf::Text subtitle("GALACTIC DEFENSE", font, 14);
			sf::FloatRect subBounds = subtitle.getLocalBounds();
			subtitle.setOrigin(subBounds.width / 2, subBounds.height / 2);
			subtitle.setPosition(400, 150);
			subtitle.setFillColor(sf::Color(180, 180, 180));
			window.draw(subtitle);

			//Survival option bounds
			sf::Text survivalOption("SURVIVAL MODE", font, 18);
			sf::FloatRect survBounds = survivalOption.getLocalBounds();

			//Survival glow when selected
			if (selectedOption == 0)
			{
				sf::Text survGlow("SURVIVAL MODE", font, 18);
				survGlow.setOrigin(survBounds.width / 2, survBounds.height / 2);
				survGlow.setPosition(400, 280);
				survGlow.setFillColor(sf::Color(255, 50, 50, 60));
				window.draw(survGlow);
			}

			survivalOption.setOrigin(survBounds.width / 2, survBounds.height / 2);
			survivalOption.setPosition(400, 280);
			survivalOption.setFillColor(selectedOption == 0 ? sf::Color(255, 50, 50) : sf::Color::White);
			window.draw(survivalOption);

			//Arcade option bounds
			sf::Text arcadeOption("ARCADE MODE", font, 18);
			sf::FloatRect arcBounds = arcadeOption.getLocalBounds();

			//Arcade glow when selected
			if (selectedOption == 1)
			{
				sf::Text arcGlow("ARCADE MODE", font, 18);
				arcGlow.setOrigin(arcBounds.width / 2, arcBounds.height / 2);
				arcGlow.setPosition(400, 330);
				arcGlow.setFillColor(sf::Color(255, 50, 50, 60));
				window.draw(arcGlow);
			}

			arcadeOption.setOrigin(arcBounds.width / 2, arcBounds.height / 2);
			arcadeOption.setPosition(400, 330);
			arcadeOption.setFillColor(selectedOption == 1 ? sf::Color(255, 50, 50) : sf::Color::White);
			window.draw(arcadeOption);

			//Instructions
			sf::Text instructions("UP/DOWN to select  |  ENTER to start", font, 10);
			sf::FloatRect instBounds = instructions.getLocalBounds();
			instructions.setOrigin(instBounds.width / 2, instBounds.height / 2);
			instructions.setPosition(400, 550);
			instructions.setFillColor(sf::Color(120, 120, 120));
			window.draw(instructions);
		}
		//YOU WIN SCREEN - for Arcade mode when game is won
		else if (gameMode == 1 && arcadeManager.isGameWon())
		{
			static bool winSoundPlayed = false;
			bossMusic.stop();
			menuMusic.stop();
			if (!winSoundPlayed)
			{
				winCheerSound.play();
				winSoundPlayed = true;
			}

			//Dark vignette overlay
			sf::RectangleShape winOverlay(sf::Vector2f(800, 600));
			winOverlay.setFillColor(sf::Color(0, 0, 0, 200));
			window.draw(winOverlay);

			//Trophy animation - 5 frames, fire animates continuously
			static float trophyTimer = 0.0f;
			static int trophyFrame = 0;
			trophyTimer += deltaTime;
			if (trophyTimer > 0.15f)
			{
				trophyTimer = 0.0f;
				trophyFrame = (trophyFrame + 1) % 5;  //Only 5 frames (0-4)
			}

			int FRAME_WIDTH = 71;

			sf::Sprite trophy(trophyTexture);
			trophy.setTextureRect(sf::IntRect(trophyFrame * FRAME_WIDTH, 0, FRAME_WIDTH, 143));
			trophy.setScale(1.0f, 1.0f);
			trophy.setOrigin(FRAME_WIDTH / 2.0f, 71.0f);
			trophy.setPosition(390, 80);
			window.draw(trophy);

			//Gold box for content
			sf::RectangleShape winBox(sf::Vector2f(380, 180));
			winBox.setFillColor(sf::Color(20, 20, 40, 220));
			winBox.setOutlineColor(sf::Color(255, 215, 0, 80));  //Gold outline
			winBox.setOutlineThickness(2);
			winBox.setOrigin(190, 90);
			winBox.setPosition(400, 280);
			window.draw(winBox);

			//Glow layers behind title
			sf::Text winGlow1("YOU WIN!", font, 28);
			sf::FloatRect wBounds = winGlow1.getLocalBounds();
			winGlow1.setOrigin(wBounds.width / 2, wBounds.height / 2);
			winGlow1.setPosition(400, 250);
			winGlow1.setFillColor(sf::Color(255, 215, 0, 40));  //Faint gold glow
			window.draw(winGlow1);

			sf::Text winGlow2("YOU WIN!", font, 28);
			winGlow2.setOrigin(wBounds.width / 2, wBounds.height / 2);
			winGlow2.setPosition(400, 250);
			winGlow2.setFillColor(sf::Color(255, 215, 0, 80));  //Brighter gold glow
			window.draw(winGlow2);

			//Main title
			sf::Text winText("YOU WIN!", font, 28);
			winText.setOrigin(wBounds.width / 2, wBounds.height / 2);
			winText.setPosition(400, 250);
			winText.setFillColor(sf::Color(255, 215, 0));  //Gold
			window.draw(winText);

			//Final score display
			sf::Text winScore("Final Score: " + std::to_string(displayedScore), font, 16);
			sf::FloatRect wsBounds = winScore.getLocalBounds();
			winScore.setOrigin(wsBounds.width / 2, wsBounds.height / 2);
			winScore.setPosition(400, 300);
			winScore.setFillColor(sf::Color::White);
			window.draw(winScore);

			//Return instruction
			sf::Text winReturn("Press ENTER to return to menu", font, 12);
			sf::FloatRect wrBounds = winReturn.getLocalBounds();
			winReturn.setOrigin(wrBounds.width / 2, wrBounds.height / 2);
			winReturn.setPosition(400, 460);
			winReturn.setFillColor(sf::Color(150, 150, 150));
			window.draw(winReturn);
		}

		//GAMEPLAY SCREEN (Playing or Paused)
		else if (gameState.isPlaying() || gameState.isPaused())
		{
			//Update HUD strings
			scoreText.setString("Score: " + std::to_string(scoreManager.getScore()));       //Score display

			std::string weaponName = "Standard";                                             //Weapon name
			if (player.getCurrentWeapon() == 1) weaponName = "Spread";
			if (player.getCurrentWeapon() == 2) weaponName = "Piercing";
			weaponText.setString("Weapon: " + weaponName);                                   //Weapon display

			shieldText.setString("Shield: " + std::to_string(player.getShieldHits()));       //Shield display
			empText.setString("EMP: " + std::to_string(player.getEmpCount()));               //EMP display

			//Wave/Level display
			if (gameMode == 0)
				waveText.setString("Wave: " + std::to_string(waveManager.getWaveNumber()));  //Survival wave
			else
			{
				if (arcadeManager.isGameWon())
					waveText.setString("YOU WIN!");                                          //Victory
				else if (arcadeManager.isLevelComplete())
					waveText.setString("Level Complete!");                                   //Level done
				else if (arcadeManager.isBossActive())
					waveText.setString("BOSS FIGHT!");                                       //Boss active
				else
					waveText.setString("Level " + std::to_string(arcadeManager.getCurrentLevel()) + " - Wave " + std::to_string(arcadeManager.getCurrentWave()));  //Arcade progress
			}

			//Multiplier display
			if (scoreManager.getMultiplier() > 1)
				multiplierText.setString("x" + std::to_string(scoreManager.getMultiplier()));  //Show multiplier
			else
				multiplierText.setString("");                                                  //Hide when 1x

			//Wave announcement overlay
			if (showWaveAnnounce)
			{
				sf::RectangleShape darkOverlay(sf::Vector2f(800, 600));
				darkOverlay.setFillColor(sf::Color(0, 0, 0, 100));
				window.draw(darkOverlay);

				//Main "WAVE X" text
				sf::Text waveAnnounce(announcedText, font, 28);
				//Color based on announcement type
				if (announcedText == "CRUISER")
					waveAnnounce.setFillColor(sf::Color(100, 255, 100, 200));      //Green
				else if (announcedText == "TWIN CANNONS")
					waveAnnounce.setFillColor(sf::Color(180, 100, 255, 200));      //Purple
				else if (announcedText == "MOTHERSHIP")
					waveAnnounce.setFillColor(sf::Color(255, 105, 180, 200));      //Hot Pink
				else if (announcedText.find("LEVEL") != std::string::npos)
					waveAnnounce.setFillColor(sf::Color(255, 255, 100, 200));      //Yellow
				else
					waveAnnounce.setFillColor(sf::Color(255, 255, 255, 200));      //White
				sf::FloatRect waBounds = waveAnnounce.getLocalBounds();
				waveAnnounce.setOrigin(waBounds.width / 2, waBounds.height / 2);
				waveAnnounce.setPosition(410, 300);
				window.draw(waveAnnounce);

				//Warning text below for Vipers/Seekers
				if (announcedWave == 1 || announcedWave == 5 || announcedWave == 8)
				{
					sf::Text warningText;
					warningText.setFont(font);
					warningText.setCharacterSize(14);

					if (announcedWave == 1)
					{
						warningText.setString("DRONES INCOMING");
						warningText.setFillColor(sf::Color(100, 255, 100, 200));  //Green
					}
					else if (announcedWave == 5)
					{
						warningText.setString("VIPERS INCOMING");
						warningText.setFillColor(sf::Color(255, 105, 180, 200));  //Hot Pink
					}
					else
					{
						warningText.setString("SEEKERS DETECTED");
						warningText.setFillColor(sf::Color(255, 100, 100, 200));  //Red
					}

					sf::FloatRect warnBounds = warningText.getLocalBounds();
					warningText.setOrigin(warnBounds.width / 2, warnBounds.height / 2);
					warningText.setPosition(400, 330);
					window.draw(warningText);
				}
			}

			//Mothership laser warning - expanding ring like EMP
			if (gameMode == 1 && arcadeManager.isBossActive() && arcadeManager.getCurrentLevel() == 3)
			{
				Mothership* m = dynamic_cast<Mothership*>(arcadeManager.getBoss());
				if (m && m->isLaserWarning())
				{
					float timer = m->getWarningTimer();
					float progress = 1.0f - timer;  //0 to 1 over 1 second
					float radius = progress * 450.0f;  //Expands to 450px
					float alpha = (1.0f - progress) * 180;  //Fades out

					sf::CircleShape warningRing(radius);
					warningRing.setOrigin(radius, radius);
					warningRing.setPosition(m->getX(), m->getY());
					warningRing.setFillColor(sf::Color::Transparent);
					warningRing.setOutlineColor(sf::Color(255, 0, 0, (int)alpha));  //Pure red
					warningRing.setOutlineThickness(8.0f - progress * 6.0f);
					window.draw(warningRing);

					//Inner glow
					sf::CircleShape innerGlow(radius * 0.6f);
					innerGlow.setOrigin(radius * 0.6f, radius * 0.6f);
					innerGlow.setPosition(m->getX(), m->getY());
					innerGlow.setFillColor(sf::Color(255, 0, 0, (int)(alpha * 0.15f)));  //More transparent (was 0.4f)
					window.draw(innerGlow);
				}
			}

			//Draw game objects
			player.draw(window);                                 //Player ship
			enemyManager.drawAll(window);                        //All enemies
			bulletManager.drawAll(window);                       //All bullets
			powerUpManager.drawAll(window);                      //All power-ups
			asteroidManager.drawAll(window);                     //All asteroids

			//HUD background box
			sf::RectangleShape hudBox(sf::Vector2f(800, 60));
			hudBox.setFillColor(sf::Color(15, 15, 30, 200));
			hudBox.setOutlineColor(sf::Color(80, 20, 20, 120));
			hudBox.setOutlineThickness(1);
			hudBox.setPosition(0, 0);
			window.draw(hudBox);

			//Row 1
			window.draw(scoreText);
			window.draw(weaponText);
			window.draw(shieldText);
			window.draw(empText);

			//Row 2
			window.draw(livesLabel);
			for (int h = 0; h < player.getLives(); h++)
			{
				sf::Sprite heart(heartTexture);
				heart.setScale(0.035f, 0.035f);
				heart.setPosition(85 + h * 22, 22);
				heart.setColor(sf::Color::Red);
				window.draw(heart);
			}
			window.draw(multiplierText);
			window.draw(waveText);

			//Dash bar - inside HUD box
			sf::Text dashLabel;
			dashLabel.setFont(font);
			dashLabel.setCharacterSize(10);
			dashLabel.setFillColor(sf::Color::White);
			dashLabel.setString("Dash:");
			dashLabel.setPosition(290, 44);

			dashBarBg.setSize(sf::Vector2f(80, 6));
			dashBarBg.setFillColor(sf::Color(0, 0, 0, 200));
			dashBarBg.setOutlineColor(sf::Color::Red);
			dashBarBg.setOutlineThickness(1);
			dashBarBg.setPosition(345, 45);

			float dashPercent = 1.0f - (player.getDashCooldown() / 3.0f);
			if (dashPercent < 0) dashPercent = 0;
			if (dashPercent > 1) dashPercent = 1;

			dashBarFill.setSize(sf::Vector2f(80 * dashPercent, 6));
			dashBarFill.setFillColor(sf::Color(255, 255, 0, 200));
			dashBarFill.setPosition(345, 45);

			window.draw(dashLabel);
			window.draw(dashBarBg);
			window.draw(dashBarFill);

			if (gameMode == 1 && !arcadeManager.isGameWon())     //Boss (only if not won)
			{
				if (!bossEMPFlash || (int)(bossEMPFlashTimer * 30) % 2 == 0)
				{
					arcadeManager.drawBoss(window);
				}

				//BOSS HEALTH BAR
				Enemy* boss = arcadeManager.getBoss();
				if (boss)
				{
					float hpPercent = (float)boss->getHealth() / boss->getMaxHealth();
					if (hpPercent < 0) hpPercent = 0;

					int level = arcadeManager.getCurrentLevel();
					sf::Color barColor;
					if (level == 1)      barColor = sf::Color::Green;        //Cruiser
					else if (level == 2) barColor = sf::Color(180, 100, 255); //Purple - Twin Cannons
					else                 barColor = sf::Color(255, 105, 180); //Hot Pink - Mothership

					//Bar background
					sf::RectangleShape hpBg(sf::Vector2f(300, 14));
					hpBg.setFillColor(sf::Color(30, 30, 30, 200));
					hpBg.setOutlineColor(sf::Color::White);
					hpBg.setOutlineThickness(1);
					hpBg.setPosition(250, 70);
					window.draw(hpBg);

					//Health fill
					sf::RectangleShape hpFill(sf::Vector2f(300 * hpPercent, 14));
					hpFill.setFillColor(barColor);
					hpFill.setPosition(250, 70);
					window.draw(hpFill);
				}
			}

			for (int i = 0; i < explosionCount; i++)
				explosions[i]->draw(window);

			//Pause overlay
			if (gameState.isPaused())
			{
				//Dark overlay
				sf::RectangleShape pauseOverlay(sf::Vector2f(800, 600));
				pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));
				window.draw(pauseOverlay);

				//Pause title
				sf::Text pauseText("PAUSED", font, 36);
				sf::FloatRect pBounds = pauseText.getLocalBounds();
				pauseText.setOrigin(pBounds.width / 2, pBounds.height / 2);
				pauseText.setPosition(400, 200);
				pauseText.setFillColor(sf::Color::White);
				window.draw(pauseText);

				//Resume option
				sf::Text resumeText("Press ESC to Resume", font, 18);
				sf::FloatRect rBounds = resumeText.getLocalBounds();
				resumeText.setOrigin(rBounds.width / 2, rBounds.height / 2);
				resumeText.setPosition(400, 280);
				resumeText.setFillColor(sf::Color::Green);
				window.draw(resumeText);

				//Quit option
				sf::Text quitText("Press Q to Quit to Menu", font, 18);
				sf::FloatRect qBounds = quitText.getLocalBounds();
				quitText.setOrigin(qBounds.width / 2, qBounds.height / 2);
				quitText.setPosition(400, 330);
				quitText.setFillColor(sf::Color::Red);
				window.draw(quitText);
			}
		}
		//GAME OVER SCREEN
		else if (gameState.isGameOver())
		{
			//Dark vignette overlay
			sf::RectangleShape goOverlay(sf::Vector2f(800, 600));
			goOverlay.setFillColor(sf::Color(0, 0, 0, 200));
			window.draw(goOverlay);

			//Blue box - sized for content
			sf::RectangleShape goBox(sf::Vector2f(380, 200));
			goBox.setFillColor(sf::Color(20, 20, 40, 220));
			goBox.setOutlineColor(sf::Color(100, 100, 200, 80));
			goBox.setOutlineThickness(2);
			goBox.setOrigin(190, 100);
			goBox.setPosition(400, 280);
			window.draw(goBox);

			//Glow layers behind title
			sf::Text glow1("GAME OVER", font, 28);
			sf::FloatRect goBounds = glow1.getLocalBounds();
			glow1.setOrigin(goBounds.width / 2, goBounds.height / 2);
			glow1.setPosition(400, 210);
			glow1.setFillColor(sf::Color(255, 30, 30, 60));
			window.draw(glow1);

			sf::Text glow2("GAME OVER", font, 28);
			glow2.setOrigin(goBounds.width / 2, goBounds.height / 2);
			glow2.setPosition(400, 210);
			glow2.setFillColor(sf::Color(255, 100, 100, 120));
			window.draw(glow2);

			//Game Over title
			sf::Text gameOverText("GAME OVER", font, 28);
			gameOverText.setOrigin(goBounds.width / 2, goBounds.height / 2);
			gameOverText.setPosition(400, 210);
			gameOverText.setFillColor(sf::Color(255, 50, 50));
			window.draw(gameOverText);

			//Final score
			sf::Text finalScoreText("Final Score: " + std::to_string(displayedScore), font, 16);
			sf::FloatRect fsBounds = finalScoreText.getLocalBounds();
			finalScoreText.setOrigin(fsBounds.width / 2, fsBounds.height / 2);
			finalScoreText.setPosition(400, 275);
			finalScoreText.setFillColor(sf::Color::White);
			window.draw(finalScoreText);

			//Wave reached
			if (gameMode == 0)
			{
				sf::Text waveReached("Wave Reached: " + std::to_string(waveManager.getWaveNumber()), font, 16);
				sf::FloatRect wrBounds = waveReached.getLocalBounds();
				waveReached.setOrigin(wrBounds.width / 2, wrBounds.height / 2);
				waveReached.setPosition(400, 315);
				waveReached.setFillColor(sf::Color::White);
				window.draw(waveReached);
			}
			else
			{
				sf::Text levelReached("Level: " + std::to_string(arcadeManager.getCurrentLevel()) + " - Wave " + std::to_string(arcadeManager.getCurrentWave()), font, 16);
				sf::FloatRect lrBounds = levelReached.getLocalBounds();
				levelReached.setOrigin(lrBounds.width / 2, lrBounds.height / 2);
				levelReached.setPosition(400, 315);
				levelReached.setFillColor(sf::Color::White);
				window.draw(levelReached);
			}
			//Return instruction - below the box
			sf::Text returnText("Press ENTER to return to menu", font, 12);
			sf::FloatRect retBounds = returnText.getLocalBounds();
			returnText.setOrigin(retBounds.width / 2, retBounds.height / 2);
			returnText.setPosition(400, 460);
			returnText.setFillColor(sf::Color(150, 150, 150));
			window.draw(returnText);
		}
		window.display();
		window.setView(window.getDefaultView());

	}

	return 0;
}

