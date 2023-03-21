#include "mainState.h"

MainState::MainState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures)
{
    srand(time(0));

    this->buttons["play"] = new Button(this->window, this->window->getSize().x / 2, 300, 1, &this->textures["play_idle"]);
    this->buttons["play"]->setTexture(&this->textures["play_idle"], &this->textures["play_hover"], &this->textures["play_active"]);
    this->buttons["play"]->setOrigin(this->textures["play_idle"].getSize().x / 2, this->textures["play_idle"].getSize().y / 2);

    this->buttons["exit"] = new Button(this->window, this->window->getSize().x / 2, 510, 1, &this->textures["play_idle"]);
    this->buttons["exit"]->setTexture(&this->textures["exit_idle"], &this->textures["exit_hover"], &this->textures["exit_active"]);
    this->buttons["exit"]->setOrigin(this->textures["exit_idle"].getSize().x / 2, this->textures["exit_idle"].getSize().y / 2);

    this->tbSeed = new TextBox(this->window, 300, 50, this->window->getSize().x / 2 - 150, 100, 8);
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

    this->updateMouse();
    this->filterSeedStr();

    this->tbSeed->update(mousePressedLeft, this->mousePosition, this->find(pressedKeys, sf::Keyboard::BackSpace));

    for (auto it : this->buttons)
    {
        it.second->update(this->mousePosition, mousePressedLeft);

        if (it.second->state == Button::states::HOVER)
        {
            it.second->setScale(1.1f, 1.1f);

        }

        else 
        {
            it.second->setScale(1.f, 1.f);
        }    
    }

    if (this->find(realisedKeys, sf::Keyboard::Key::Escape) ||
        (mousePressedLeft && this->buttons["exit"]->isHover(this->mousePosition)))
    {
        delete this->states.top();
        this->states.pop();
        return;
    }

    if (mousePressedLeft && this->buttons["play"]->isHover(this->mousePosition))
    {
        this->states.push(new GameState(State::typeState::GAMESTATE, this->window, this->states, this->textures, std::stoi(this->tbSeed->getStr())));
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