#include "tilemap.h"

Tilemap::Tilemap(sf::RenderWindow* window, std::map<std::string, sf::Texture>& textures, int width, int height, int frequency, int octaves, int seedGame)
    : window(window), width(width), height(height)
{
    const siv::PerlinNoise::seed_type seed = seedGame;
    this->perlinNoise = new siv::PerlinNoise(seed);
    this->tiles = {
        {0, textures["tile_water"]},
        {1, textures["tile_sand"]},
        {2, textures["tile_ground"]},
        {3, textures["tile_mountain"]},
        {4, textures["tile_snow"]}
    };

    for (int y = 0; y < this->height; y++)
    {
        this->map.push_back(std::vector<int>());
        this->mapUnits.push_back(std::vector<int>());

        for (int x = 0; x < this->width; x++)
        {
            float ix = (float)x / this->width;
            float iy = (float)y / this->height;

            double noise = this->perlinNoise->octave2D_01((ix * frequency), (iy * frequency), octaves);
            // std::cout << noise << "\n";

            if (noise <= 0.25)
            {
                noise = 0; // water
            }

            else if (noise > 0.25 && noise <= 0.3)
            {
                noise = 1; // sand
            }

            else if (noise > 0.3 && noise <= 0.65)
            {
                noise = 2; // ground
            }

            else if (noise > 0.65 && noise <= 0.76)
            {
                noise = 3; // mountain
            }

            else if (noise > 0.76)
            {
                noise = 4; // snow
            }

            this->map[y].push_back((int)noise);
            this->mapUnits[y].push_back(0);
        }
    }
}

Tilemap::~Tilemap()
{
    delete this->perlinNoise;
}

void Tilemap::generateNew()
{

}

void Tilemap::renderGame(sf::View view)
{
    sf::View old = this->window->getView();
    this->window->setView(view);

    sf::Sprite sprite;
    sf::Vector2f pos(0, 0);

    for (int y = 0; y < this->map.size(); y++)
    {
        for (int x = 0; x < this->map[y].size(); x++)
        {
            sprite.setTexture(this->tiles[this->map[y][x]]);
            sprite.setPosition(pos);
            this->window->draw(sprite);
            pos.x += 64;
        }

        pos.y += 64;
        pos.x = 0;
    }

    this->window->setView(old);
}

void Tilemap::renderMini(sf::View view)
{
    sf::View old = this->window->getView();
    this->window->setView(view);

    sf::Sprite sprite;
    sf::Vector2f pos(0, 0);

    for (int y = 0; y < this->map.size(); y++)
    {
        for (int x = 0; x < this->map[y].size(); x++)
        {
            sprite.setTexture(this->tiles[this->map[y][x]]);
            sprite.setScale(0.15, 0.15);
            sprite.setPosition(pos);
            this->window->draw(sprite);
            pos.x += sprite.getTexture()->getSize().x * sprite.getScale().x;
        }

        pos.y += sprite.getTexture()->getSize().y * sprite.getScale().y;
        pos.x = 0;
    }

    this->window->setView(old);
}
