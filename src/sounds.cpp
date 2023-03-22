#include "sounds.h"

namespace sounds 
{
    const std::string rootPath = "resources/sounds/";

    std::map<std::string, sf::SoundBuffer> buffers;
    std::vector<sf::Sound> sounds;
    std::map<std::string, float> volumes;

    void load(std::string name, float volume)
    {
        if (volume == -1)
        {
            volume = 10;
        }

        volumes[name] = volume;
        buffers[name].loadFromFile(rootPath + name + ".wav");
    }

    float getVolume(std::string name)
    {
        return volumes[name];
    }

    void play(std::string name, float volume, bool repeat)
    {
        if (sounds.size() == 0)
        {
            sounds.push_back(sf::Sound());
            sounds[0].setBuffer(buffers[name]);
            sounds[0].setVolume(volume);
            sounds[0].play();
        }

        else 
        {
            int location = -1;

            for (int i = 0; i < sounds.size(); i++)
            {
                // if (sounds[i].getStatus() != sf::Sound::Playing)
                // {
                //     sounds.erase(sounds.begin() + i);
                // }

                if (sounds[i].getStatus() != sf::Sound::Playing && location == -1)
                {
                    location = i;
                    break;
                }
            }

            if (location != -1)
            {
                sounds[location].setBuffer(buffers[name]);
                sounds[location].setVolume(volume);
                sounds[location].play();
            }

            else 
            {
                sounds.push_back(sf::Sound());
                sounds[sounds.size() - 1].setBuffer(buffers[name]);
                sounds[sounds.size() - 1].setVolume(volume);
                sounds[sounds.size() - 1].play();   
            }
        }
    }
};