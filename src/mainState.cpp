#include "mainState.h"

MainState::MainState(sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(window, states, textures)
{

}

MainState::~MainState()
{
    delete this->window;
}

void MainState::update()
{

}

void MainState::render()
{

}