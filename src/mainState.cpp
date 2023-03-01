#include "mainState.h"

MainState::MainState(sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(window, states, textures)
{
    this->buttons["play"] = new Button(this->window, 100, 100, 1, &this->textures["play_idle"]);
}

MainState::~MainState()
{
    delete this->window;
}

void MainState::update(bool mousePressed)
{
    this->updateMouse();

    for (auto it : this->buttons)
    {
        it.second->update(this->mousePosition, mousePressed);

        if (it.second->isHover(this->mousePosition) && mousePressed && it.first == "play")
        {
            
        }
    }
}

void MainState::render()
{
    for (auto it : this->buttons)
    {
        it.second->render();
    }
}