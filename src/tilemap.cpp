#include "tilemap.h"

Tilemap::Tilemap(sf::RenderWindow* window, std::map<std::string, sf::Texture>& textures, int width, int height, int frequency, int octaves, int seedGame)
    : window(window), width(width), height(height), animSleep(10), soundSleep(150)
{
    srand(time(0));

    this->currentSound = this->soundSleep;

    this->tileKeys = {
        {"water", {0, 7}},
        {"sand", {8, 8}},
        {"ground", {9, 12}},
        {"mountain", {13, 13}},
        {"snow", {14, 14}}
    };

    const siv::PerlinNoise::seed_type seed = seedGame;
    this->perlinNoise = new siv::PerlinNoise(seed);
    this->currentAnim = 0;

    this->tiles = {
        {0, textures["tile_water_1"]},
        {1, textures["tile_water_2"]},
        {2, textures["tile_water_3"]},
        {3, textures["tile_water_4"]},
        {4, textures["tile_water_5"]},
        {5, textures["tile_water_6"]},
        {6, textures["tile_water_7"]},
        {7, textures["tile_water_8"]},
        {8, textures["tile_sand"]},
        {9, textures["tile_ground_1"]},
        {10, textures["tile_ground_2"]},
        {11, textures["tile_ground_3"]},
        {12, textures["tile_ground_4"]},
        {13, textures["tile_mountain"]},
        {14, textures["tile_snow"]}
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

void Tilemap::checkWater(int x, int y, float& distX, float& distY)
{
    sf::Vector2f cameraPos = this->window->getView().getCenter();

    // x
    if (x > cameraPos.x)
    {
        distX = x - cameraPos.x;
    }

    else 
    {
        distX = cameraPos.x - x;
    }

    // y
    if (y > cameraPos.y)
    {
        distY = y - cameraPos.y;
    }

    else 
    {
        distY = cameraPos.y - y;
    }
}

void Tilemap::renderGame(sf::View view)
{
    sf::View old = this->window->getView();
    this->window->setView(view);

    sf::Sprite sprite;
    sf::Vector2f pos(0, 0);

    bool waterPlay = false;
    int minDistX = 10000000;
    int minDistY = 10000000;
    int xPlay;
    int yPlay;

    for (int y = 0; y < this->map.size(); y++)
    {
        for (int x = 0; x < this->map[y].size(); x++)
        {
            if (this->currentAnim <= 0)
            {
                if (this->map[y][x] < this->tileKeys["water"].second)
                {
                    this->map[y][x]++;
                }

                else if (this->map[y][x] == this->tileKeys["water"].second)
                {
                    this->map[y][x] = this->tileKeys["water"].first;
                }

                this->currentAnim = this->animSleep;
            }

            else 
            {
                this->currentAnim--;
            }

            if (this->currentSound <= 0)
            {
                if (this->map[y][x] <= this->tileKeys["water"].second && this->map[y][x] >= this->tileKeys["water"].first && 
                    Camera::cameraShape.getGlobalBounds().contains(sf::Vector2f(x * 64, y * 64)))
                {
                    waterPlay = true;

                    float distX;
                    float distY;

                    this->checkWater(x, y, distX, distY);

                    if (distX < minDistX && distY < minDistY)
                    {
                        minDistX = distX;
                        minDistY = distY;

                        xPlay = x;
                        yPlay = y;
                    }
                }
            }

            sprite.setTexture(this->tiles[this->map[y][x]]);
            sprite.setPosition(pos);
            this->window->draw(sprite);
            pos.x += 64;
        }

        pos.y += 64;
        pos.x = 0;
    }

    if (waterPlay)
    {
        int hitSound = randomNumbers::getRandomNum(1, 3);
        std::string str = "water_" + std::string(std::to_string(hitSound));
        sounds::play(str, sounds::getVolume(str), false, true, xPlay * 64, yPlay * 64);

        this->currentSound = this->soundSleep;
    }

    this->currentSound--;
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
