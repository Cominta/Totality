#include "engine.h"

Engine::Engine(sf::RenderWindow* window)
    : window(window)
{
    this->loadTextures();

    this->states.push(new MainState(this->window, this->states, this->textures));

    this->mousePressed = false;
}

Engine::~Engine()
{
    
}

void Engine::loadTextures()
{
    std::string rootPath = "resources/";

    std::vector<std::string> paths = {
        rootPath + "buttons/mainstate/play_idle.png",
        rootPath + "tiles/ground.png"
    };

    std::vector<std::string> names = {
        "play_idle",
        "tile_ground"
    };

    for (int i = 0; i < paths.size(); i++)
    {
        sf::Texture texture;
        texture.loadFromFile(paths[i]);

        this->textures[names[i]] = texture;
    }
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
                this->mousePressed = true;
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
    }
}

void Engine::update()
{
    this->updateSFML();

    this->states.top()->update(this->mousePressed, this->pressedKeys, this->realisedKeys);

    this->mousePressed = false;
    this->pressedKeys.clear();
    this->realisedKeys.clear();
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
        this->update();

        if (this->states.empty())
        {
            this->window->close();
            break;
        }

        this->render();
    }
}