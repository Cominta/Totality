#include "tilemap.h"

Tilemap::Tilemap(sf::RenderWindow* window, std::map<std::string, sf::Texture>& textures, int width, int height, int frequency, int octaves)
    : window(window), width(width), height(height)
{
    const siv::PerlinNoise::seed_type seed = 12334;
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

            if (noise <= 0.2)
            {
                noise = 0; // water
            }

            else if (noise > 0.2 && noise <= 0.25)
            {
                noise = 1; // sand
            }

            else if (noise > 0.25 && noise <= 0.6)
            {
                noise = 2; // ground
            }

            else if (noise > 0.6 && noise <= 0.7)
            {
                noise = 3; // mountain
            }

            else if (noise > 0.7)
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

}

void Tilemap::generateNew()
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
            sprite.setTexture(this->tiles[this->map[y][x]]);
            sprite.setPosition(pos);
            this->window->draw(sprite);
            pos.x += 64;
        }

        pos.y += 64;
        pos.x = 0;
    }
}