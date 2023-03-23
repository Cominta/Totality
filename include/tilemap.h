#pragma once 

#include <map>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "PerlinNoise.hpp"
#include "viewMap.h"
#include "sounds.h"
#include "randomNums.h"

class Tilemap 
{
    private:
        const int width;
        const int height;

        sf::RenderWindow* window;
        siv::PerlinNoise* perlinNoise;

        // тайлы представляются в виде чисел
        // 0 - земля
        // -1 - вода
        // 1 - гора
        // вот так вот просто

        const int animSleep;
        int currentAnim;

        const int soundSleep;
        int currentSound;

        std::map<int, sf::Texture> tiles;
        void checkWater(int x, int y, float& distX, float& distY);

    public:
        std::vector<std::vector<int>> map;
        std::vector<std::vector<int>> mapUnits;
        std::map<std::string, std::pair<int, int>> tileKeys;
        
        Tilemap(sf::RenderWindow* window, std::map<std::string, sf::Texture>& textures, int width, int height, int frequency, int octaves, int seedGame);
        ~Tilemap();

        const int getWidth() {return this->width;}
        const int getHeight() {return this->height;}
        
        int getTile(int x, int y) {return this->map[y][x];};

        void renderGame(sf::View view);
        void renderMini(sf::View view);
};