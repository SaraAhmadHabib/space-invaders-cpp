#pragma once

enum GameState
{
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class GameStateManager
{
private:
    GameState currentState;

public:
    GameStateManager();                    //Constructor, starts at MENU
    void setState(GameState newState);     //Switch to a new state
    GameState getState();                  //Returns current state
    bool isPlaying();                      //True if state is PLAYING
    bool isPaused();                       //True if state is PAUSED
    bool isMenu();                         //True if state is MENU
    bool isGameOver();                     //True if state is GAME_OVER
};