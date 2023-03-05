#include "gameState.h"

GameState::GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures), sizeMapX(100), sizeMapY(100)
{
    this->tilemap = new Tilemap(this->window, this->textures, this->sizeMapX, this->sizeMapY, 3, 16);

    for (int i = 0; i < 2; i++)
    {
        this->units.push_back(new BaseUnit(this->window));
        this->units[i]->setPosition(100, 150 * i);
        this->units[i]->getHitbox()->update();
        // this->quadtree->insert(this->units[i]);
    }

    // this->quadtree = new Quadtree(this->window, 4, this->sizeMapX * 64 / 2, this->sizeMapY * 64 / 2, this->sizeMapX * 64, this->sizeMapY * 64);
    this->quadtree = new Quadtree(this->window, 4, this->window->getSize().x / 2, this->window->getSize().y / 2, this->window->getSize().x, this->window->getSize().y);
}

GameState::~GameState()
{
    delete this->tilemap;
    
    for (auto& unit : this->units)
    {
        delete unit;
    }
}

void GameState::update(bool mousePressedLeft, bool MousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys)
{
    this->quadtree->clear();

    for (auto& unit : this->units)
    {
        this->quadtree->insert(unit);
    }

    for (auto& unit : this->units)
    {
        std::vector<BaseUnit*> unitsFound;
        HitboxSquare* next = unit->getHitbox();
        unit->update(mousePressedLeft, pressedKeys, realisedKeys);
        next->setSize(next->width + 30, next->height + 30);
        next->setOrigin(next->width / 2, next->height / 2);

        this->quadtree->query(next, unitsFound);

        next->setSize(next->width - 30, next->height - 30);
        next->setOrigin(next->width / 2, next->height / 2);

        unit->moveTo(unitsFound);
        std::cout << unitsFound.size() << " ";
    }

    std::cout << "\n";

    // std::cout << this->units[0]->getHitbox()->y << " " << this->units[1]->getHitbox()->y << "\n";

    if (this->find(realisedKeys, sf::Keyboard::Key::Escape))
    {
        delete this->states.top();
        this->states.pop();
        return;
    }
}

void GameState::render()
{
    this->tilemap->render();

    for (auto& unit : this->units)
    {
        unit->render();
    }

    this->quadtree->render();
}