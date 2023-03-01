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
    delete this->window;
    
}

void Engine::loadTextures()
{
    std::string rootPath = "resources/";

    std::vector<std::string> paths = {
        rootPath + "buttons/mainstate/play_idle.png"
    };

    std::vector<std::string> names = {
        "play_idle"
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
    }
}

void Engine::update()
{
    this->updateSFML();

    if (!this->states.empty())
    {
        this->states.top()->update(this->mousePressed);
    }

    this->mousePressed = false;
}

void Engine::render()
{
    this->window->clear();

    if (!this->states.empty())
    {
        this->states.top()->render();
    }

    this->window->display();
}

void Engine::start()
{
    while (this->window->isOpen())
    {
        this->update();
        this->render();
    }
}