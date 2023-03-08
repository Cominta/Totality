#pragma once 

#include <map>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "PerlinNoise.hpp"

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

        std::map<int, sf::Texture> tiles;

    public:
        std::vector<std::vector<int>> map;
        std::vector<std::vector<int>> mapUnits;
        
        Tilemap(sf::RenderWindow* window, std::map<std::string, sf::Texture>& textures, int width, int height, int frequency, int octaves, int seedGame);
        ~Tilemap();

        const int getWidth() {return this->width;}
        const int getHeight() {return this->height;}
        
        void generateNew();
        int getTile(int x, int y) {return this->map[y][x];};

        void renderGame(sf::View view);
        void renderMini(sf::View view);
};