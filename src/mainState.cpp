#include "mainState.h"

MainState::MainState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures)
{
    this->buttons["play"] = new Button(this->window, 100, 100, 1, &this->textures["play_idle"]);
}

MainState::~MainState()
{

}

void MainState::update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys)
{
    if (this->find(realisedKeys, sf::Keyboard::Key::Escape))
    {
        delete this->states.top();
        this->states.pop();
        return;
    }

    this->updateMouse();

    for (auto it : this->buttons)
    {
        it.second->update(this->mousePosition, mousePressed);

        if (it.second->isHover(this->mousePosition) && mousePressed && it.first == "play")
        {
            this->states.push(new GameState(State::typeState::GAMESTATE, this->window, this->states, this->textures));
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