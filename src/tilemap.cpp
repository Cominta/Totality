#include "tilemap.h"

Tilemap::Tilemap(sf::RenderWindow* window, std::map<std::string, sf::Texture>& textures, int width, int height, int frequency, int octaves, int seedGame)
    : window(window), width(width), height(height)
{
    srand(time(0));

    this->tileKeys = {
        {"water", {0, 0}},
        {"sand", {1, 1}},
        {"ground", {2, 3}},
        {"mountain", {4, 4}},
        {"snow", {5, 5}}
    };

    const siv::PerlinNoise::seed_type seed = seedGame;
    this->perlinNoise = new siv::PerlinNoise(seed);
    this->tiles = {
        {0, textures["tile_water"]},
        {1, textures["tile_sand"]},
        {2, textures["tile_ground_1"]},
        {3, textures["tile_ground_2"]},
        {4, textures["tile_mountain"]},
        {5, textures["tile_snow"]}
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
            int tile;
            // std::cout << noise << "\n";

            if (noise <= 0.25) // water
            {
                tile = abs(rand() % (this->tileKeys["water"].second + 1 - this->tileKeys["water"].first) + this->tileKeys["water"].first);
            }

            else if (noise > 0.25 && noise <= 0.3) // sand
            {
                tile = abs(rand() % (this->tileKeys["sand"].second + 1 - this->tileKeys["sand"].first) + this->tileKeys["sand"].first);
            }

            else if (noise > 0.3 && noise <= 0.65) // ground
            {
                tile = abs(rand() % (this->tileKeys["ground"].second + 1 - this->tileKeys["ground"].first) + this->tileKeys["ground"].first);
            }

            else if (noise > 0.65 && noise <= 0.76) // mountain
            {
                tile = abs(rand() % (this->tileKeys["mountain"].second + 1 - this->tileKeys["mountain"].first) + this->tileKeys["mountain"].first);
            }

            else if (noise > 0.76) // snow
            {
                tile = abs(rand() % (this->tileKeys["snow"].second + 1 - this->tileKeys["snow"].first) + this->tileKeys["snow"].first);
            }

            this->map[y].push_back((int)tile);
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
