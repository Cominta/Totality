#include "sounds.h"

namespace sounds 
{
    const std::string rootPath = "resources/sounds/";

    std::map<std::string, sf::SoundBuffer> buffers;
    std::vector<std::pair<sf::Sound, std::string>> sounds; // для коротких звуков по типу удара 
    std::vector<std::pair<sf::Sound, std::string>> musics; // для фоновых длинных звуков
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
            sounds.push_back(std::pair<sf::Sound, std::string> (sf::Sound(), name));
            sounds[0].first.setBuffer(buffers[name]);
            sounds[0].first.setVolume(volume);
            sounds[0].first.play();
        }

        else 
        {
            int location = -1;

            for (int i = 0; i < sounds.size(); i++)
            {
                if (sounds[i].first.getStatus() != sf::Sound::Playing && location == -1)
                {
                    location = i;
                    break;
                }
            }

            if (location != -1)
            {
                sounds.push_back(std::pair<sf::Sound, std::string> (sf::Sound(), name));
                sounds[location].first.setBuffer(buffers[name]);
                sounds[location].first.setVolume(volume);
                sounds[location].first.play();
            }

            else 
            {
                sounds.push_back(std::pair<sf::Sound, std::string> (sf::Sound(), name));
                sounds[sounds.size() - 1].second = name;
                sounds[sounds.size() - 1].first.setBuffer(buffers[name]);
                sounds[sounds.size() - 1].first.setVolume(volume);
                sounds[sounds.size() - 1].first.setLoop(repeat);
                sounds[sounds.size() - 1].first.play();   
            }
        }
    }

    void playMusic(std::string name)
    {
        if (musics.size() == 0)
        {
            musics.push_back(std::pair<sf::Sound, std::string> (sf::Sound(), name));
            musics[0].first.setBuffer(buffers[name]);
            musics[0].first.setVolume(volumes[name]);
            musics[0].first.play();
        }

        else 
        {
            int location = -1;

            for (int i = 0; i < sounds.size(); i++)
            {
                if (musics[i].first.getStatus() != sf::Sound::Playing && location == -1)
                {
                    location = i;
                    break;
                }
            }

            if (location != -1)
            {
                musics.push_back(std::pair<sf::Sound, std::string> (sf::Sound(), name));
                musics[location].first.setBuffer(buffers[name]);
                musics[location].first.setVolume(volumes[name]);
                musics[location].first.setLoop(true);
                musics[location].first.play();
            }

            else 
            {
                musics.push_back(std::pair<sf::Sound, std::string> (sf::Sound(), name));
                musics[musics.size() - 1].second = name;
                musics[musics.size() - 1].first.setBuffer(buffers[name]);
                musics[musics.size() - 1].first.setVolume(volumes[name]);
                musics[musics.size() - 1].first.setLoop(true);
                musics[musics.size() - 1].first.play();   
            }
        }
    }

    void stopMusic(std::string name)
    {
        for (int i = 0; i < musics.size(); i++)
        {
            if (musics[i].second == name)
            {
                musics[i].first.stop();
            }
        }
    }

    void setWindow(sf::RenderWindow* _window)
    {
        window = _window;
    }
};