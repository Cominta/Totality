#include "engine.h"

Engine::Engine(sf::RenderWindow* window)
    : window(window)
{
    this->loadTextures();

    this->states.push(new MainState(State::typeState::MAINSTATE, this->window, this->states, this->textures));

    this->mousePressedLeft = false;
    this->mousePressedRight = false;
    this->typedString = "";
}

Engine::~Engine()
{
    
}

void Engine::loadTextures()
{
    std::string rootPath = "resources/";

    std::vector<std::string> paths = {
        rootPath + "buttons/mainstate/play_idle.jpg",
        rootPath + "buttons/mainstate/play_hover.jpg",
        rootPath + "buttons/mainstate/play_active.jpg",
        rootPath + "buttons/gamestate/UnitButton_Idle.png",
        rootPath + "buttons/gamestate/BaseUnitAddButton_Idle.png",
        rootPath + "buttons/gamestate/ArcherUnitAddButton_Idle.png",
        rootPath + "buttons/gamestate/Team_Red.png",
        rootPath + "buttons/gamestate/Team_Blue.png",
        rootPath + "buttons/gamestate/Team_Green.png",
        rootPath + "buttons/gamestate/Team_Yellow.png",
        rootPath + "tiles/ground_1.png",
        rootPath + "tiles/ground_2.png",
        rootPath + "tiles/ground_3.png",
        rootPath + "tiles/ground_4.png",
        rootPath + "tiles/water_1.png",
        rootPath + "tiles/water_2.png",
        rootPath + "tiles/water_3.png",
        rootPath + "tiles/water_4.png",
        rootPath + "tiles/water_5.png",
        rootPath + "tiles/water_6.png",
        rootPath + "tiles/water_7.png",
        rootPath + "tiles/water_8.png",
        rootPath + "tiles/mountain.png",
        rootPath + "tiles/sand.png",
        rootPath + "tiles/snow.png",
        rootPath + "blood.png",
        rootPath + "dead.png"
    };

    std::vector<std::string> names = {
        "play_idle",
        "play_hover",
        "play_active",
        "UnitButton_Idle",
        "BaseUnitAddButton_Idle",
        "ArcherUnitAddButton_Idle",
        "Team_Red",
        "Team_Blue",
        "Team_Green",
        "Team_Yellow",
        "tile_ground_1",
        "tile_ground_2",
        "tile_ground_3",
        "tile_ground_4",
        "tile_water_1",
        "tile_water_2",
        "tile_water_3",
        "tile_water_4",
        "tile_water_5",
        "tile_water_6",
        "tile_water_7",
        "tile_water_8",
        "tile_mountain",
        "tile_sand",
        "tile_snow",
        "blood",
        "dead"
    };

    for (int i = 0; i < paths.size(); i++)
    {
        sf::Texture texture;
        texture.loadFromFile(paths[i]);

        this->textures[names[i]] = texture;
    }
}

void Engine::updateDt()
{
    this->dt = this->clock.restart().asSeconds();
}

void Engine::updateSFML()
{
    while (this->window->pollEvent(this->sfEvent))
    {
        if (this->sfEvent.type == sf::Event::Closed)
        {
            this->window->close();
        }

        if (this->sfEvent.type == sf::Event::MouseButtonReleased)
        {
            if (this->sfEvent.mouseButton.button == sf::Mouse::Left)
            {
                this->mousePressedLeft = true;
            }

            else if (this->sfEvent.mouseButton.button == sf::Mouse::Right)
            {
                this->mousePressedRight = true;
            }
        }

        if (this->sfEvent.type == sf::Event::KeyReleased)
        {
            this->realisedKeys.push_back(this->sfEvent.key.code);
        }

        if (this->sfEvent.type == sf::Event::KeyPressed)
        {
            this->pressedKeys.push_back(this->sfEvent.key.code);
        }

        if (this->sfEvent.type == sf::Event::MouseWheelScrolled && this->sfEvent.mouseWheelScroll.delta != 0)
        {
            this->mouseScroll = this->sfEvent.mouseWheelScroll.delta;
        }

        if (this->sfEvent.type == sf::Event::TextEntered && this->states.top()->type == State::typeState::MAINSTATE)
        {
            ((MainState*)this->states.top())->getTb()->typedOn(this->sfEvent);
        }
    }
}

void Engine::update()
{
    this->updateSFML();

    // int sum = 0; // for test delta time

    // for (int i = 0; i < 10000000; i++)
    // {
    //     sum += i;
    // }

    if (this->states.top()->type == State::typeState::MAINSTATE)
    {
        this->states.top()->update(this->mousePressedLeft, this->mousePressedRight, this->pressedKeys, this->realisedKeys);
    }

    else if (this->states.top()->type == State::typeState::GAMESTATE) 
    {
        this->states.top()->update(this->mousePressedLeft, this->mousePressedRight, this->pressedKeys, this->realisedKeys, this->mouseScroll, this->dt);
    }

    this->mousePressedLeft = false;
    this->mousePressedRight = false;
    this->pressedKeys.clear();
    this->realisedKeys.clear();
    this->mouseScroll = 0;
}

void Engine::render()
{
    this->window->clear();

    this->states.top()->render();

    this->window->display();
}

void Engine::start()
{
    while (this->window->isOpen())
    {
        this->updateDt();
        this->update();

        if (this->states.empty())
        {
            this->window->close();
            break;
        }

        this->render();
    }
}