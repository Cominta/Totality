
#include "BaseUnit.h"

void BaseUnit::update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, std::vector<std::vector<int>>& mapUnits)
{
	if (mousePressed)
	{
		if (mousePressed)
		{
			sf::Vector2i mousepos = sf::Mouse::getPosition(*this->window);
			this->window->mapCoordsToPixel(sf::Vector2f(mousepos), this->window->getView());
			if (unit.getGlobalBounds().contains(sf::Vector2f(mousepos)))
			{
				this->b_active = true;
				this->setOutlineColor(255, 0 ,0);
			}
			else if (!find(pressedKeys, sf::Keyboard::LShift) || !find(pressedKeys, sf::Keyboard::RShift))
			{
				this->b_active = false;
				this->setOutlineColor(0, 0, 0);
			}
		}
	}
	else if (find(realisedKeys, sf::Keyboard::Enter))
	{
		if (isActiv())
		{
			sf::Vector2i mousepos = sf::Mouse::getPosition(*this->window);
			this->window->mapPixelToCoords(mousepos);
			this->setMove(mousepos);
			this->setIsMoving(true);
			this->currentSpeed = 0;
		}
	}
}

void BaseUnit::moveTo(std::vector<std::vector<int>>& mapUnits)
{
	if (!this->b_moving)
	{
		return;
	}

	if (this->currentSpeed != 0)
	{
		this->currentSpeed--;
		return;
	}

	int oldX = this->xMap;
	int oldY = this->yMap;
	mapUnits[this->yMap][this->xMap] = 0;

	if (this->wayEnd.x > this->xMap)
	{
		this->xMap++;
	}

	if (this->wayEnd.x < this->xMap)
	{
		this->xMap--;
	}

	if (this->wayEnd.y > this->yMap)
	{
		this->yMap++;
	}

	if (this->wayEnd.y < this->yMap)
	{
		this->yMap--;
	}

	if (mapUnits[this->yMap][this->xMap] == 1)
	{
		this->xMap = oldX;
		this->yMap = oldY;

		this->b_moving = false;
		return;
	}

	mapUnits[this->yMap][this->xMap] = 1;
	this->unit.setPosition(this->xMap * 64 + 32, this->yMap * 64 + 32);

	if (this->wayEnd.x == this->xMap && this->wayEnd.y == this->yMap)
	{
		this->b_moving = false;
	}

	this->currentSpeed = 200;
}

void BaseUnit::render()
{
	this->window->draw(this->unit);
}
