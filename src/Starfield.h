#pragma once
#include <SFML/Graphics.hpp>

class Starfield
{
private:
    struct Star
    {
        sf::CircleShape shape;   //Tiny dot
        float speed;             //Scroll speed
    };

    Star** layer1;    //Far layer — many small dim dots, slowest
    Star** layer2;    //Mid layer — medium dots, medium speed
    Star** layer3;    //Near layer — fewer bright dots, fastest
    int count1, count2, count3;  //Stars per layer

public:
    Starfield();       //Constructor — creates all stars
    ~Starfield();      //Destructor — cleans up
    void update(float deltaTime);    //Scroll all layers downward
    void draw(sf::RenderWindow& window);  //Draw all stars
};