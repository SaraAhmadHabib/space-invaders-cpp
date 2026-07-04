#include "GameStateManager.h"

GameStateManager::GameStateManager()
{
    currentState = MENU;  //Start at main menu
}

void GameStateManager::setState(GameState newState)
{
    currentState = newState;  //Switch to new state
}

GameState GameStateManager::getState()
{
    return currentState;  //Return current state
}

bool GameStateManager::isPlaying()
{
    return currentState == PLAYING;
}

bool GameStateManager::isPaused()
{
    return currentState == PAUSED;
}

bool GameStateManager::isMenu()
{
    return currentState == MENU;
}

bool GameStateManager::isGameOver()
{
    return currentState == GAME_OVER;
}