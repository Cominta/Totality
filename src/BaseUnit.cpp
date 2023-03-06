
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

void BaseUnit::predictPath()
{
	int xPath = this->xMap;
	int yPath = this->yMap;

	int previousDirectX = 0;
	int previousDirectY = 0;

	this->path.clear();

	while (true)
	{
		int directX = 0;
		int directY = 0;

		if (this->wayEnd.x > xPath)
		{
			directX = 1;
			xPath++;
		}

		if (this->wayEnd.x < xPath)
		{
			directX = -1;
			xPath--;
		}

		if (this->wayEnd.y > yPath)
		{
			directY = 1;
			yPath++;
		}

		if (this->wayEnd.y < yPath)
		{
			directY = -1;
			yPath--;
		}

		sf::RectangleShape shape(sf::Vector2f(5, 32));

		if ((directX != previousDirectX || directY != previousDirectY) && this->path.size() != 0)
		{
			this->path[this->path.size() - 1].setSize(sf::Vector2f(32, 32));
			this->path[this->path.size() - 1].setOrigin(this->path[this->path.size() - 1].getSize().x / 2, this->path[this->path.size() - 1].getSize().y / 2);
			this->path[this->path.size() - 1].setRotation(0);
		}

		shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
		shape.setPosition(sf::Vector2f(xPath * 64 + 32, yPath * 64 + 32));

		if (directY == -1 && directX == -1) // top - left
		{
			shape.setRotation(-45);
		}

		else if (directY == -1 && directX == 1) // top - right
		{
			shape.setRotation(45);
		}

		else if (directY == 1 && directX == -1) // bottom - left
		{
			shape.setRotation(-135);
		}

		else if (directY == 1 && directX == 1) // bottom - right
		{
			shape.setRotation(135);
		}

		else if (directY == 0) // left - right
		{
			shape.setRotation(90 * directX);
		}

		else if (directX == 0) // top - bottom
		{
			shape.setRotation(180 * directY);
		}

		this->path.push_back(shape);

		if (xPath == this->wayEnd.x && yPath == this->wayEnd.y)
		{
			break;
		}

		previousDirectX = directX;
		previousDirectY = directY;
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
	this->predictPath();

	if (this->wayEnd.x == this->xMap && this->wayEnd.y == this->yMap)
	{
		this->path.clear();
		this->b_moving = false;
	}

	this->currentSpeed = 200;
}

void BaseUnit::render()
{
	if (this->b_moving)
	{
		for (int i = 0; i < this->path.size(); i++)
		{
			this->window->draw(this->path[i]);
			// std::cout << this->path[i].getPosition().x << " " << this->path[i].getPosition().y << " " << i << "\n";
		}
	}

	this->window->draw(this->unit);
}
