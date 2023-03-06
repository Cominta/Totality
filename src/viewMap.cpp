#include "viewMap.h"

Camera::Camera(sf::RenderWindow* window, int sideX, int sideY) 
    : window(window), sideX(sideX), sideY(sideY)
{
    this->scrollSpeed = 50;
}
Camera::~Camera()
{

}

void Camera::update(sf::Vector2f mousePosition, int mouseScroll)
{
    sf::View view {this->window->getView()};
    sf::Vector2f currentSize {this->window->getSize()};

    //(mouse scrolling)-------------------------
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

    if (mouseScroll > 0 && mouseScroll <= 1) // прокрутка вверх
    {
        view.zoom(0.95);
    }
    else if (mouseScroll < 0 && mouseScroll >= -1) // прокрутка вниз
    {
        view.zoom(1.05);
    }

    //(changing camera view)-------------------------

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		view.rotate(15);
	}
 
    view.move(this->currentSpeedX, this->currentSpeedY);
    this->window->setView(view);

    currentSpeedX = 0;
    currentSpeedY = 0;
}