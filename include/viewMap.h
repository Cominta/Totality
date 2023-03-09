#pragma once
#include "SFML/Graphics.hpp"
#include "iostream"

class Camera
{
private:
    sf::RenderWindow* window;
    float currentSpeedX;
    float currentSpeedY;
    float deltaZoom = 0;
    float currentZoom = 1;
    float scrollSpeed = 100;
    const int sideX;
    const int sideY;

public:
    Camera(sf::RenderWindow* window, int sideX, int sideY);
    ~Camera();

    void update(sf::Vector2f mousePosition, int mouseScroll, int width, int height, float dt);
};
