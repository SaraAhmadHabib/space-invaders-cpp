#pragma once

class ScoreManager
{
private:
    int score;              //Player score
    int multiplier;         //Current multiplier (1x, 2x, 4x)
    float chainTimer;       //Time since last kill
    const float CHAIN_WINDOW = 3.0f;  //Seconds to maintain chain

public:
    ScoreManager();          //Constructor — sets score, multiplier, timer to 0/1/0
    void addKill(int points);  //Called when enemy dies — adds points with multiplier
    void update(float deltaTime);  //Counts down chain timer, resets multiplier
    void resetMultiplier();   //Resets multiplier to 1x (on player damage)
    int getScore() const;     //Returns current score
    int getMultiplier() const;  //Returns current multiplier
    void reset();  //Reset score to 0
};