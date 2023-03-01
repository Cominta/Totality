#pragma once 

#include <vector>
#include <map>
#include "SFML/Graphics.hpp"
#include <iostream>

class Tilemap 
{
    private:
        const int width;
        const int height;

        sf::RenderWindow* window;

        // тайлы представляются в виде чисел
        // 0 - земля
        // -1 - вода
        // 1 - гора
        // вот так вот просто

        std::vector<std::vector<int>> map;
        std::map<int, sf::Texture> tiles;

    public:
        Tilemap(sf::RenderWindow* window, std::map<std::string, sf::Texture>& textures, int width, int height);
        ~Tilemap();

        void render();
};