#include "viewMap.h"

sf::RectangleShape Camera::cameraShape;

Camera::Camera(sf::RenderWindow* window, int sideX, int sideY) 
    : window(window), sideX(sideX), sideY(sideY)
{
    this->scrollSpeed = 1000;
    this->cameraShape.setSize(sf::Vector2f(this->window->getSize()));
    this->cameraShape.setOrigin(this->cameraShape.getSize().x / 2, this->cameraShape.getSize().y / 2);
    this->cameraShape.setFillColor(sf::Color::Transparent);
}
Camera::~Camera()
{

}

void Camera::update(sf::Vector2f mousePosition, int mouseScroll, int width, int height, float dt)
{
    const float MIN_ZOOM = 0.9;
    const float MAX_ZOOM = 3.35;

    sf::View view {this->window->getView()};
    sf::Vector2f currentSize {this->window->getSize()};
    int mapWidth = width * 64;
    int mapHeight = height * 64;

    // MAP SCROLL
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
    { 
        this->currentSpeedX = -this->scrollSpeed; 
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
    { 
        this->currentSpeedX = this->scrollSpeed; 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
    { 
        this->currentSpeedY = this->scrollSpeed; 
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
    {  
        this->currentSpeedY = -this->scrollSpeed; 
    }

    // CAMERA LIMIT X
    if (view.getCenter().x - view.getSize().x / 2 < 0) {
        view.setCenter(view.getSize().x / 2, view.getCenter().y);
    }
    if (view.getCenter().x + view.getSize().x / 2 > mapWidth) {
        view.setCenter(mapWidth - view.getSize().x / 2, view.getCenter().y);
    }

    // CAMERA LIMIT Y
    if (view.getCenter().y - view.getSize().y / 2 < 0) {
        view.setCenter(view.getCenter().x, view.getSize().y / 2);
    }
    if (view.getCenter().y + view.getSize().y / 2 > mapHeight) {
        view.setCenter(view.getCenter().x, mapHeight - view.getSize().y / 2);
    }

    // CAMERA ZOOM
    if (mouseScroll != 0)
    {
        if (mouseScroll > 0) //MouseWheel up
        {
            deltaZoom = 0.95;
        }
        else if (mouseScroll < 0) //MouseWheel down
        {
            deltaZoom = 1.05;
        }
        view.zoom(deltaZoom);
        currentZoom *= deltaZoom;
    }

    // CAMERA LIMIT ZOOM    
    if (currentZoom < MIN_ZOOM) {
        currentZoom = MIN_ZOOM;
        view.setSize(currentSize * currentZoom);
    }
    else if (currentZoom > MAX_ZOOM) {
        currentZoom = MAX_ZOOM;
        view.setSize(currentSize * currentZoom);
    }

    view.move(this->currentSpeedX * dt, this->currentSpeedY * dt);
    this->window->setView(view);

    this->cameraShape.setPosition(view.getCenter());
    this->cameraShape.setSize(view.getSize());
    this->cameraShape.setOrigin(this->cameraShape.getSize().x / 2, this->cameraShape.getSize().y / 2);

    currentSpeedX = 0;
    currentSpeedY = 0;
    
}