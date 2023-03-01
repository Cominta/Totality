#include "tilemap.h"

Tilemap::Tilemap(sf::RenderWindow* window, std::map<std::string, sf::Texture>& textures, int width, int height)
    : window(window), width(width), height(height)
{
    this->tiles = {
        {0, textures["tile_ground"]}
    };

    for (int y = 0; y < this->height; y++)
    {
        this->map.push_back(std::vector<int>());

        for (int x = 0; x < this->width; x++)
        {
            this->map[y].push_back(0);
        }
    }
}

Tilemap::~Tilemap()
{

}

void Tilemap::render()
{
    sf::Sprite sprite;
    sf::Vector2f pos(0, 0);

    for (int y = 0; y < this->map.size(); y++)
    {
        for (int x = 0; x < this->map[y].size(); x++)
        {
            sprite.setTexture(this->tiles[0]);
            sprite.setPosition(pos);
            this->window->draw(sprite);
            pos.x += 64;
        }

        pos.y += 64;
        pos.x = 0;
    }
}