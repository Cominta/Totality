#pragma once

#include "SFML/Audio.hpp"
#include <map>
#include <iostream>

namespace sounds
{
    void load(std::string name, float volume = -1);
    float getVolume(std::string name);
    void play(std::string name, float volume = -1, bool repeat = false);
};  