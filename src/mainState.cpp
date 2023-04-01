#include "mainState.h"

MainState::MainState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures)
{
    srand(time(0));
    
    this->background.setSize(sf::Vector2f(this->window->getSize()));
    this->background.setTexture(&this->textures["background"]);

    this->buttons["play"] = new Button(this->window, 300, 450, 0.7, &this->textures["play_idle"]);
    this->buttons["play"]->setTexture(&this->textures["play_idle"], &this->textures["play_hover"], &this->textures["play_active"]);
    this->buttons["play"]->setOrigin(this->textures["play_idle"].getSize().x / 2, this->textures["play_idle"].getSize().y / 2);

    this->buttons["exit"] = new Button(this->window, 300, 610, 0.7, &this->textures["play_idle"]);
    this->buttons["exit"]->setTexture(&this->textures["exit_idle"], &this->textures["exit_hover"], &this->textures["exit_active"]);
    this->buttons["exit"]->setOrigin(this->textures["exit_idle"].getSize().x / 2, this->textures["exit_idle"].getSize().y / 2);

    this->tbSeed = new TextBox(this->window, 300, 50, 150, 300, 8);
    int seed = rand() % 1000000000;
    
    std::string str = std::to_string(seed);
    this->tbSeed->setStr(str);

    this->font.loadFromFile("resources/fonts/mainmenu.ttf");

    this->title.setFont(this->font);
    this->title.setCharacterSize(100);
    this->title.setStyle(sf::Text::Bold);
    this->title.setFillColor(sf::Color::Black);
    this->title.setPosition(1200, 150);
    this->title.setString("TOTALITY");

    this->credits.setFont(this->font);
    this->credits.setCharacterSize(20);
    this->credits.setStyle(sf::Text::Bold);
    this->credits.setFillColor(sf::Color::Black);
    this->credits.setPosition(1200, 270);
    this->credits.setString("Team-lead: Roman Zaitsev \nCoders: Ilya Idir, Roman Zaitsev, Andrey Kostin,\nDenis Hutsul");

    this->version.setFont(this->font);
    this->version.setCharacterSize(30);
    this->version.setStyle(sf::Text::Bold);
    this->version.setFillColor(sf::Color::White);
    this->version.setPosition(this->window->getSize().x - 400, this->window->getSize().y - 100);
    this->version.setString("Version: " + std::string(VERSION));
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
            it.second->setScale(0.8f, 0.8f);

        }

        else 
        {
            it.second->setScale(0.7f, 0.7f);
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
    this->window->draw(this->background);
    this->window->draw(this->credits);
    this->window->draw(this->title);
    this->window->draw(this->version);

    this->tbSeed->render();

    for (auto it : this->buttons)
    {
        it.second->render();
    }
}