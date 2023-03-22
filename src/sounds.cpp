#include "sounds.h"

namespace sounds 
{
    const std::string rootPath = "resources/sounds/";

    std::map<std::string, sf::SoundBuffer> buffers;
    std::vector<sf::Sound> sounds;
    std::map<std::string, float> volumes;
    sf::RenderWindow* window;

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

    void play(std::string name, float volume, bool repeat, bool solid, float x, float y)
    {
        if (volume == -1)
        {
            volume = volumes[name];
        }

        if (solid)
        {
            const int maxDist = 1500;
            int minDist = 300;
            float volumeFactor = 0;
            float distX;
            float distY;
            float dist;

            // Volume factor = MinDistance / (MinDistance + Attenuation * (max(Distance, MinDistance) - MinDistance))

            // x
            if (x > window->getView().getCenter().x)
            {
                distX = x - window->getView().getCenter().x;
            }

            else 
            {
                distX = window->getView().getCenter().x - x;
            }

            // y
            if (y > window->getView().getCenter().y)
            {
                distY = y - window->getView().getCenter().y;
            }

            else 
            {
                distY = window->getView().getCenter().y - y;
            }

            if (distX <= maxDist && distY <= maxDist)
            {
                dist = sqrt(pow(distX, 2) + pow(distY, 2));
                volumeFactor = minDist / (minDist + 1.0f * (std::max((int)dist, minDist) - minDist));
                
                volume *= volumeFactor;
            }

            else 
            {
                return;
            }
        }

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

    void setWindow(sf::RenderWindow* _window)
    {
        window = _window;
    }
};