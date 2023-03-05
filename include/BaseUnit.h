#pragma once

#include "SFML\Graphics.hpp"
#include <iostream>

class BaseUnit
{
private:
	sf::CircleShape unit;
	sf::Vector2i wayEnd;
	sf::Vector2i speed;
	sf::RenderWindow* window;
	bool b_active;
	bool b_moving;

public:
	BaseUnit(sf::RenderWindow *_window)
	{
		unit.setRadius(50.f);
		unit.setOrigin(50.f, 50.f);
		unit.setFillColor(sf::Color(100, 100, 100));
		unit.setOutlineThickness(2.5f);
		unit.setOutlineColor(sf::Color(0, 0, 0));
		unit.move(50.f, 50.f);
		wayEnd.x = 0;
		wayEnd.y = 0;
		speed.x = 0;
		speed.y = 0;
		b_active = false;
		b_moving = false;
		window = _window;

		this->wayEnd.x = this->unit.getPosition().x;
		this->wayEnd.y = this->unit.getPosition().y;
	}


	~BaseUnit()
	{}

	void moveTo(std::vector<BaseUnit*> foundRange);

	void setActive(bool _active)
	{
		b_active = _active;
	}
	bool isActiv() const
	{
		return b_active;
	}

	void setIsMoving(bool status)
	{
		b_moving = status;
	}
	bool isMoving() const
	{
		return b_moving;
	}

	void setMove(float x, float y)
	{
		wayEnd.x = x;
		wayEnd.y = y;
	}
	void setMove(sf::Vector2i cord)
	{
		wayEnd.x = cord.x;
		wayEnd.y = cord.y;
	}

	void setOutlineThickness(float wide)
	{
		unit.setOutlineThickness(wide);
	}
	void setOutlineColor(sf::Uint8 R, sf::Uint8 G, sf::Uint8 B)
	{
		unit.setOutlineColor(sf::Color(R, G, B));
	}
	void setFillColor(sf::Uint8 R, sf::Uint8 G, sf::Uint8 B)
	{
		unit.setFillColor(sf::Color(R, G, B));
	}
	void setRadius(float radius)
	{
		unit.setRadius(radius);
	}
	bool GlobalBoundContainCheck(sf::Vector2i cord)
	{
		return unit.getGlobalBounds().contains(sf::Vector2f(cord));
	}
	void move(float X, float Y)
	{
		unit.move(X, Y);
	}

	void setPosition(float x, float y)
	{
		this->unit.setPosition(x, y);
	}

	void update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys);
	bool find(std::vector<int> keys, int item)
	{
	    for (int i = 0; i < keys.size(); i++)
	    {
	        if (keys[i] == item)
	        {
	            return true;
	        }
	    }

	    return false;
	}

	void render();
};
