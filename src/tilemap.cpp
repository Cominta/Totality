#include "tilemap.h"

Tilemap::Tilemap(sf::RenderWindow* window, std::map<std::string, sf::Texture>& textures, int width, int height, int frequency, int octaves, int seedGame)
    : window(window), width(width), height(height), animSleep(10), soundSleep(150)
{
    srand(time(0));

    this->currentSound = this->soundSleep;

    this->tileKeys = {
        {"water", {0, 7}},
        {"sand", {8, 8}},
        {"crossSand", {1000000, 12000000}},
        {"ground", {9, 12}},
        {"crossGround", {15, 80033}},
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
        {14, textures["tile_snow"]},
        {15, textures["g_bottom_sand"]}, 
        {16, textures["g_left_sand"]}, 
        {17, textures["g_right_sand"]}, 
        {19, textures["g_top_sand"]}, 
        {31, textures["g_bottom_left_sand"]}, 
        {32, textures["g_bottom_right_sand"]}, 
        {33, textures["g_left_right_sand"]}, 
        {34, textures["g_bottom_top_sand"]},
        {35, textures["g_left_top_sand"]}, 
        {36, textures["g_right_top_sand"]},
        {100, textures["g_bottom_mount"]},
        {200, textures["g_left_mount"]},
        {400, textures["g_right_mount"]},
        {800, textures["g_top_mount"]},
        {300, textures["g_bottom_left_mount"]},
        {500, textures["g_bottom_right_mount"]},
        {600, textures["g_left_right_mount"]},
        {900, textures["g_bottom_top_mount"]},
        {1000, textures["g_left_top_mount"]},
        {1200, textures["g_right_top_mount"]},
        {10000, textures["g_bottom_water"]}, 
        {10016, textures["g_bottom_water_left_sand"]}, 
        {10017, textures["g_bottom_water_right_sand"]}, 
        {10019, textures["g_bottom_water_top_sand"]},
        {10033, textures["g_bottom_water_left_right_sand"]}, 
        {10035, textures["g_bottom_water_left_top_sand"]},
        {10036, textures["g_bottom_water_right_top_sand"]},
        {20000, textures["g_left_water"]}, 
        {20015, textures["g_left_water_bottom_sand"]}, 
        {20017, textures["g_left_water_right_sand"]}, 
        {20019, textures["g_left_water_top_sand"]},
        {20032, textures["g_left_water_bottom_right_sand"]}, 
        {20034, textures["g_left_water_bottom_top_sand"]},
        {20036, textures["g_left_water_right_top_sand"]},
        {40000, textures["g_right_water"]}, 
        {40015, textures["g_right_water_bottom_sand"]}, 
        {40016, textures["g_right_water_left_sand"]}, 
        {40019, textures["g_right_water_top_sand"]},
        {40031, textures["g_right_water_bottom_left_sand"]}, 
        {40034, textures["g_right_water_bottom_top_sand"]},
        {40035, textures["g_right_water_left_top_sand"]},
        {80000, textures["g_top_water"]},
        {80015, textures["g_top_water_bottom_sand"]}, 
        {80016, textures["g_top_water_left_sand"]}, 
        {80017, textures["g_top_water_right_sand"]},
        {80031, textures["g_top_water_bottom_left_sand"]}, 
        {80032, textures["g_top_water_bottom_right_sand"]},
        {80033, textures["g_top_water_left_right_sand"]},
        {1000000, textures["s_bottom_water"]}, 
        {2000000, textures["s_left_water"]}, 
        {4000000, textures["s_right_water"]}, 
        {8000000, textures["s_top_water"]},
        {3000000, textures["s_bottom_left_water"]}, 
        {5000000, textures["s_bottom_right_water"]},
        {6000000, textures["s_left_right_water"]},
        {9000000, textures["s_bottom_top_water"]},
        {10000000, textures["s_left_top_water"]}, 
        {12000000, textures["s_right_top_water"]}
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
<<<<<<< HEAD
    this->mapAfterprocces();
=======

>>>>>>> 9a6b89a4821f523a916f9de7d7a98f944cf2121a
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
            if ((x * 64 + 64 < Camera::cameraShape.getPosition().x - Camera::cameraShape.getSize().x / 2 || 
                 x * 64 - 64 > Camera::cameraShape.getPosition().x + Camera::cameraShape.getSize().x / 2) &&
                (y * 64 + 64 < Camera::cameraShape.getPosition().y - Camera::cameraShape.getSize().y / 2 ||
                 y * 64 - 64 > Camera::cameraShape.getPosition().y + Camera::cameraShape.getSize().y / 2))
            {
                pos.x += 64;
                continue;
            }

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

void Tilemap::mapAfterprocces()
{
    int tile_key = 0;
    for (int y = 0; y < this->height; y++)
    {
        for (int x = 0; x < this->width; x++)
        {
            tile_key = 0;
            int toGround = 0;
            int toWater = 0;
            if (this->map[y][x] >= 9 && this->map[y][x] <= 12 || this->map[y][x] >= 15 && this->map[y][x] < 1000000)
            {
                if (y > 0 && this->map[y - 1][x] > 12 && this->map[y - 1][x] < 15 ||
                y > 0 && this->map[y - 1][x] < 9 || y > 0 && this->map[y - 1][x] >= 1000000)
                {
                    if(this->map[y - 1][x] == 8 || this->map[y - 1][x] >= 1000000)
                    {
                        tile_key += 19;
                    }
                    else if (this->map[y - 1][x] == 13)
                    {
                        tile_key += 800;
                    }
                    else if (this->map[y - 1][x] <= 7 && this->map[y - 1][x] >= 0)
                    {
                        tile_key += 80000;
                        toWater += 1;
                    }
                }
                if (x > 0 && this->map[y][x - 1] > 12 && this->map[y][x - 1] < 15 ||
                x > 0 && this->map[y][x - 1] < 9 || x > 0 && this->map[y][x - 1] >= 1000000)
                {
                    if (this->map[y][x - 1] == 8 || this->map[y][x - 1] >= 1000000)
                    {
                        tile_key += 16;
                    }
                    else if (this->map[y][x - 1] == 13)
                    {
                        tile_key += 200;
                    }
                    else if (this->map[y][x - 1] <= 7 && this->map[y][x - 1] >= 0)
                    {
                        tile_key += 20000;
                        toWater += 1;
                    }
                }
                if (x < this->width - 1 && this->map[y][x + 1] > 12 && this->map[y][x + 1] < 15 ||
                x < this->width - 1 && this->map[y][x + 1] < 9 || x < this->width - 1 && this->map[y][x + 1] >= 1000000)
                {
                    if (this->map[y][x + 1] == 8 || this->map[y][x + 1] >= 1000000)
                    {
                        tile_key += 17;
                    }
                    else if (this->map[y][x + 1] == 13)
                    {
                        tile_key += 400;
                    }
                    else if (this->map[y][x + 1] <= 7 && this->map[y][x + 1] >= 0)
                    {
                        tile_key += 40000;
                        toWater += 1;
                    }
                }
                if(y < this->height - 1 && this->map[y + 1][x] > 12 && this->map[y + 1][x] < 15 ||
                y < this->height - 1 && this->map[y + 1][x] < 9 || y < this->height - 1 && this->map[y + 1][x] >= 1000000)
                {
                    if (this->map[y + 1][x] == 8 || this->map[y + 1][x] >= 1000000)
                    {
                        tile_key += 15;
                    }
                    else if (this->map[y + 1][x] == 13)
                    {
                        tile_key += 100;
                    }
                    else if (this->map[y + 1][x] <= 7 && this->map[y + 1][x] >= 0)
                    {
                        tile_key += 10000;
                        toWater += 1;
                    }
                }
                if (tile_key != 0 || toWater != 0)
                {
                    if (this->tiles.count(tile_key) != 0)
                    {
                        this->map[y][x] = tile_key;
                    }
                    else
                    {
                        if (tile_key % 10000 > 699)
                        {
                            this->map[y][x] = 13;
                            y -= (y >= 2 ? 2 : (y > 0 ? 1 : 0));
                        }
                        else if (tile_key % 100 > 36)
                        {
                            this->map[y][x] = 8;
                            y -= (y >= 2 ? 2 : (y > 0 ? 1 : 0));
                        }
                        else if (toWater >= 2)
                        {
                            this->map[y][x] = 0;
                            y -= (y >= 2 ? 2 : (y > 0 ? 1 : 0));
                        }
                        //else if (tile_key % 1000000 > 120200)
                        //{
                        //    this->map[y][x] = 0;
                        //    y -= (y >= 2 ? 2 : (y > 0 ? 1 : 0));
                        //}
                    }
                }
                else if (this->map[y][x] < 1000000 && this->map[y][x] >= 15)
                {
                    if (y > 0 && this->map[y - 1][x] >= 9 && this->map[y - 1][x] <= 12 ||
                     y > 0 && this->map[y - 1][x] >= 15 && this->map[y - 1][x] < 1000000)
                    {
                        toGround += 1;
                    }
                    if (x > 0 && this->map[y][x - 1] >= 9 && this->map[y][x - 1] <= 12 ||
                     x > 0 && this->map[y][x - 1] >= 15 && this->map[y][x - 1] < 1000000)
                    {
                        toGround += 1;
                    }
                    if (x < this->width - 1 && this->map[y][x + 1] >= 9 && this->map[y][x + 1] <= 12 ||
                     x < this->width - 1 && this->map[y][x + 1] >= 15 && this->map[y][x + 1] < 1000000)
                    {
                        toGround += 1;
                    }
                    if (y < this->height - 1 && this->map[y + 1][x] >= 9 && this->map[y + 1][x] <= 12 ||
                     y < this->height - 1 && this->map[y + 1][x] >= 15 && this->map[y + 1][x] < 1000000)
                    {
                        toGround += 1;
                    }
                    if (toGround == 4)
                    {
                        this->map[y][x] = 9;
                        y -= (y >= 2 ? 2 : (y > 0 ? 1 : 0));
                    }
                }
            }
            else if (this->map[y][x] == 8 || this->map[y][x] >= 1000000)
            {
                if (y > 0 && this->map[y - 1][x] != 8 && this->map[y - 1][x] < 1000000)
                {
                    //if (this->map[y - 1][x] >= 9 && this->map[y - 1][x] <= 12 ||
                    //this->map[y - 1][x] >= 15 && this->map[y - 1][x] < 1000000)
                    //{
                    //    toGround += 1;
                    //}
                    if (this->map[y - 1][x] >= 0 && this->map[y - 1][x] <= 7)
                    {
                        tile_key += 8000000;
                        toWater += 1;
                    }
                }
                if (x > 0 && this->map[y][x - 1] != 8 && this->map[y][x - 1] < 1000000)
                {
                    //if (this->map[y][x - 1] >= 9 && this->map[y][x - 1] <= 12 ||
                    //this->map[y][x - 1] >= 15 && this->map[y][x - 1] < 1000000)
                    //{
                    //    toGround += 1;
                    //}
                    if (this->map[y][x - 1] >= 0 && this->map[y][x - 1] <= 7)
                    {
                        tile_key += 2000000;
                        toWater += 1;
                    }
                }
                if (x < this->width - 1 && this->map[y][x + 1] != 8 && this->map[y][x + 1] < 1000000)
                {
                    //if (this->map[y][x + 1] >= 9 && this->map[y][x + 1] <= 12 ||
                    //this->map[y][x + 1] >=15 && this->map[y][x + 1] < 1000000)
                    //{
                    //    toGround += 1;
                    //}
                    if (this->map[y][x + 1] >= 0 && this->map[y][x + 1] <= 7)
                    {
                        tile_key += 4000000;
                        toWater += 1;
                    }
                }
                if (y < this->height - 1 && this->map[y + 1][x] != 8 && this->map[y + 1][x] < 1000000)
                {
                    //if (this->map[y + 1][x] >= 9 && this->map[y + 1][x] <= 12 ||
                    //this->map[y + 1][x] >= 15 && this->map[y + 1][x] < 1000000)
                    //{
                    //    toGround += 1;
                    //}
                    if (this->map[y + 1][x] >= 0 && this->map[y + 1][x] <= 7)
                    {
                        tile_key += 1000000;
                        toWater += 1;
                    }
                }
                if (tile_key != 0 || toGround != 0 || toWater != 0)
                {
                    if (this->tiles.count(tile_key) != 0)
                    {
                        this->map[y][x] = tile_key;
                    }
                    //else if (toGround >= 4)
                    //{
                    //    this->map[y][x] = 9;
                    //    y -= (y >= 2 ? 2 : (y > 0 ? 1 : 0));
                    //}
                    else if (toWater >= 3)
                    {
                        this->map[y][x] = 0;
                        y -= (y >= 2 ? 2 : (y > 0 ? 1 : 0));
                    }
                }
            }
        }
    }
}