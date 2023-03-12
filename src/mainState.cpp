#include "mainState.h"

MainState::MainState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures)
{
    srand(time(0));

    this->buttons["play"] = new Button(this->window, 10, 10, 1, &this->textures["play_idle"]);
    this->buttons["play"]->setTexture(&this->textures["play_idle"], &this->textures["play_hover"], &this->textures["play_active"]);

    this->tbSeed = new TextBox(this->window, 300, 50, 10, 500, 8);
    int seed = rand() % 1000000000;
    
    std::string str = std::to_string(seed);
    this->tbSeed->setStr(str);
}

MainState::~MainState()
{
    delete this->tbSeed;

    for (auto it : this->buttons)
    {
        delete it.second;
    }
}

void MainState::filterSeedStr()
{
    std::string str = this->tbSeed->getStr();

    if (str.size() == 0)
    {
        return;
    }

    std::string result = "";
    
    for (int i = 0; i < str.size(); i++)
    {
        if (std::isdigit(str[i]))
        {
            result += str[i];
        }
    }

    this->tbSeed->setStr(result);
}

void MainState::convert(int num, std::string& str)
{
    while (num != 0)
    {
        str += std::to_string(num % 10);
        num /= 10;
    }
}

void MainState::update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys)
{
    if (this->find(realisedKeys, sf::Keyboard::Key::Escape))
    {
        delete this->states.top();
        this->states.pop();
        return;
    }

    this->updateMouse();
    this->filterSeedStr();

    this->tbSeed->update(mousePressedLeft, this->mousePosition, this->find(pressedKeys, sf::Keyboard::BackSpace));

    for (auto it : this->buttons)
    {
        it.second->update(this->mousePosition, mousePressedLeft);

        if (it.second->isHover(this->mousePosition) && mousePressedLeft && it.first == "play")
        {
            this->states.push(new GameState(State::typeState::GAMESTATE, this->window, this->states, this->textures, std::stoi(this->tbSeed->getStr())));
        }
    }
}

void MainState::render()
{
    this->tbSeed->render();

    for (auto it : this->buttons)
    {
        it.second->render();
    }
}