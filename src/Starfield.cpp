#include "Starfield.h"
#include <cstdlib>

Starfield::Starfield()
{
    //Layer 1 — Far (100 small dim stars, slow)
    count1 = 100;
    layer1 = new Star * [count1];
    for (int i = 0; i < count1; i++)
    {
        layer1[i] = new Star();
        float size = 1.0f + (rand() % 2);              //1-2 pixels
        layer1[i]->shape.setRadius(size);
        int brightness = 80 + (rand() % 60);           //Dim gray
        layer1[i]->shape.setFillColor(sf::Color(brightness, brightness, brightness));
        layer1[i]->shape.setPosition(rand() % 800, rand() % 600);
        layer1[i]->speed = 30.0f + (rand() % 30);     //Slow
    }

    //Layer 2 — Mid (60 medium dots, medium speed)
    count2 = 60;
    layer2 = new Star * [count2];
    for (int i = 0; i < count2; i++)
    {
        layer2[i] = new Star();
        float size = 1.5f + (rand() % 2);              //1.5-2.5 pixels
        layer2[i]->shape.setRadius(size);
        int brightness = 130 + (rand() % 60);          //Brighter
        layer2[i]->shape.setFillColor(sf::Color(brightness, brightness, brightness));
        layer2[i]->shape.setPosition(rand() % 800, rand() % 600);
        layer2[i]->speed = 60.0f + (rand() % 40);     //Medium
    }

    //Layer 3 — Near (30 bright dots, fastest)
    count3 = 30;
    layer3 = new Star * [count3];
    for (int i = 0; i < count3; i++)
    {
        layer3[i] = new Star();
        float size = 2.0f + (rand() % 3);              //2-4 pixels
        layer3[i]->shape.setRadius(size);
        int brightness = 180 + (rand() % 75);          //Brightest
        layer3[i]->shape.setFillColor(sf::Color(brightness, brightness, brightness));
        layer3[i]->shape.setPosition(rand() % 800, rand() % 600);
        layer3[i]->speed = 100.0f + (rand() % 50);    //Fast
    }
}

Starfield::~Starfield()
{
    for (int i = 0; i < count1; i++) delete layer1[i];
    for (int i = 0; i < count2; i++) delete layer2[i];
    for (int i = 0; i < count3; i++) delete layer3[i];
    delete[] layer1;
    delete[] layer2;
    delete[] layer3;
}

void Starfield::update(float deltaTime)
{
    //Layer 1
    for (int i = 0; i < count1; i++)
    {
        layer1[i]->shape.move(0, layer1[i]->speed * deltaTime);
        if (layer1[i]->shape.getPosition().y > 600)
            layer1[i]->shape.setPosition(rand() % 800, -5.0f);
    }
    //Layer 2
    for (int i = 0; i < count2; i++)
    {
        layer2[i]->shape.move(0, layer2[i]->speed * deltaTime);
        if (layer2[i]->shape.getPosition().y > 600)
            layer2[i]->shape.setPosition(rand() % 800, -5.0f);
    }
    //Layer 3
    for (int i = 0; i < count3; i++)
    {
        layer3[i]->shape.move(0, layer3[i]->speed * deltaTime);
        if (layer3[i]->shape.getPosition().y > 600)
            layer3[i]->shape.setPosition(rand() % 800, -5.0f);
    }
}

void Starfield::draw(sf::RenderWindow& window)
{
    for (int i = 0; i < count1; i++) window.draw(layer1[i]->shape);
    for (int i = 0; i < count2; i++) window.draw(layer2[i]->shape);
    for (int i = 0; i < count3; i++) window.draw(layer3[i]->shape);
}