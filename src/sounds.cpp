#include "sounds.h"

namespace sounds 
{
    const std::string rootPath = "resources/sounds/";

    std::map<std::string, sf::Music*> sounds;
    std::map<std::string, float> volumes;

    void load(std::string name, float volume)
    {
        sounds[name] = new sf::Music();

        if (!sounds[name]->openFromFile(rootPath + name + ".wav"))
        {
            std::cout << "Sound load error: " << name << "\n";
        }

        if (volume == -1)
        {
            volume = 10.0f;
        }

        volumes[name] = volume;
    }

    float getVolume(std::string name)
    {
        return volumes[name];
    }

    void play(std::string name, float volume, bool repeat)
    {
        if (volume == -1)
        {
            volume = volumes[name];
        }

        sounds[name]->setVolume(volume);
        sounds[name]->setLoop(repeat);

        if (sounds[name]->getStatus() == 2)
        {
            return;
        }

        if (sounds[name]->getStatus() == 0)
        {
            sounds[name]->stop();
        }

        sounds[name]->play();
    }
};