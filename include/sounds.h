#pragma once

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include <map>
#include <iostream>
#include <cmath>

namespace sounds
{
    void load(std::string name, float volume = -1);
    float getVolume(std::string name);

    void play(std::string name, float volume = -1, bool repeat = false, bool solid = false, float x = -1, float y = -1);
    void playMusic(std::string name);
    void stopMusic(std::string name);

    void setWindow(sf::RenderWindow* _window);
};  