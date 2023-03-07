#include "viewMap.h"

Camera::Camera(sf::RenderWindow* window, int sideX, int sideY) 
    : window(window), sideX(sideX), sideY(sideY)
{
    this->scrollSpeed = 50;
}
Camera::~Camera()
{

}

void Camera::update(sf::Vector2f mousePosition, int mouseScroll, int width, int height)
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

    

    // CAMERA VIEW
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        view.rotate(5);
    }

    view.move(this->currentSpeedX, this->currentSpeedY);
    this->window->setView(view);

    currentSpeedX = 0;
    currentSpeedY = 0;
    
}