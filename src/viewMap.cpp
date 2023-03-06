#include "viewMap.h"

Camera::Camera(sf::RenderWindow* window, int sideX, int sideY) 
    : window(window), sideX(sideX), sideY(sideY)
{
    this->scrollSpeed = 50;
}
Camera::~Camera()
{

}

void Camera::update(sf::Vector2f mousePosition, float mouseWheel)
{
    sf::View view {this->window->getView()};
    sf::Vector2f currentSize {this->window->getSize()};

    //(mouse scrolling)-------------------------
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && view.getCenter().x > view.getSize().x / 2) 
    { 
        this->currentSpeedX = -this->scrollSpeed; 
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && view.getCenter().x < this->sideX - view.getSize().x / 2) 
    { 
        this->currentSpeedX = this->scrollSpeed; 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && view.getCenter().y < this->sideY - view.getSize().y / 2) 
    { 
        this->currentSpeedY = this->scrollSpeed; 
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && view.getCenter().y > view.getSize().y / 2) 
    {  
        this->currentSpeedY = -this->scrollSpeed; 
    }

    float zoom = 1.0f;

    // if (mouseWheel > 0) // прокрутка вверх
    // {
    //     zoom = 0.9f; // уменьшаем масштаб
    // }
    // else if (mouseWheel < 0) // прокрутка вниз
    // {
    //     zoom = 1.1f; // увеличиваем масштаб
    // }

    view.setSize(window->getSize().x / zoom, window->getSize().y / zoom);

    //(changing camera view)-------------------------

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		view.rotate(15);
	}
 
    view.move(this->currentSpeedX, this->currentSpeedY);
    this->window->setView(view);

    currentSpeedX = 0;
    currentSpeedY = 0;
}


