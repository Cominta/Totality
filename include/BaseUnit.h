#pragma once

#include "state.h"
#include <iostream>

class BaseUnit : public State
{
private:
	sf::CircleShape unit;
	sf::Vector2f wayEnd;
	bool b_active;
	bool b_moving;
public:
	BaseUnit(sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures) 
		: State(window, states, textures)
	{
		unit.setRadius(50.f);
		unit.setOrigin(50.f, 50.f);
		unit.setFillColor(sf::Color(100, 100, 100));
		unit.setOutlineThickness(2.5f);
		unit.setOutlineColor(sf::Color(0, 0, 0));
		unit.move(50.f, 50.f);
		b_active = false;
		b_moving = false;
	}

	~BaseUnit()
	{}

	void moveTo();

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
	void setMove(sf::Vector2f cord)
	{
		mousePosition.x = cord.x;
		mousePosition.y = cord.y;
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
	bool GlobalBoundContainCheck(sf::Vector2f cord)
	{
		return unit.getGlobalBounds().contains(cord);
	}
	void move(float X, float Y)
	{
		unit.move(X, Y);
	}

	void updateMouse()
	{
		this->mousePosition = this->window->mapPixelToCoords(sf::Mouse::getPosition(*(this->window)));
	}
	void update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys) override;
	void render();
};