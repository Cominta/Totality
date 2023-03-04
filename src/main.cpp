#include "engine.h"

const int WIDTH = 640;
const int HEIGHT = 480;
const int GRID_SIZE = 16;
const int GRID_WIDTH = WIDTH / GRID_SIZE;
const int GRID_HEIGHT = HEIGHT / GRID_SIZE;

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "Totality", sf::Style::Fullscreen);
    Engine eng(&window);
    eng.start();

    // // Create the window
    // sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Perlin Noise");
    
    // // Create a PerlinNoise object with a random seed
    // const siv::PerlinNoise::seed_type seed = 12345;
    // siv::PerlinNoise* perlinNoise = new siv::PerlinNoise(seed);

    // // Create a grid of squares to represent the noise
    // sf::RectangleShape rect(sf::Vector2f(GRID_SIZE, GRID_SIZE));

    // // Create a 2D array to store the noise values
    // float noiseValues[GRID_WIDTH][GRID_HEIGHT];

    // float fx = (4 / 500);
    // float fy = (4 / 500);

    // // Calculate the noise values for each point in the grid
    // for (int i = 0; i < GRID_WIDTH; i++)
    // {
    //     for (int j = 0; j < GRID_HEIGHT; j++)
    //     {
    //         // Normalize the coordinates to the range [0, 1]
    //         float x = (float)i / GRID_WIDTH;
    //         float y = (float)j / GRID_HEIGHT;

    //         // Calculate the noise value at this point
    //         float noiseValue = perlinNoise->octave2D_01((x * 8), (y * 8), 8);
    //         std::cout << noiseValue << "\n";

    //         // Store the noise value in the array
    //         noiseValues[i][j] = noiseValue;
    //     }
    // }

    // // Loop until the window is closed
    // while (window.isOpen())
    // {
    //     // Handle events
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //         {
    //             window.close();
    //         }
    //     }

    //     // Clear the window
    //     window.clear();

    //     // Draw the grid of squares
    //     for (int i = 0; i < GRID_WIDTH; i++)
    //     {
    //         for (int j = 0; j < GRID_HEIGHT; j++)
    //         {
    //             // Set the color of the square based on the noise value
    //             float noiseValue = noiseValues[i][j];
    //             sf::Color color(noiseValue * 255, noiseValue * 255, noiseValue * 255);

    //             // Set the position and color of the square
    //             rect.setPosition(i * GRID_SIZE, j * GRID_SIZE);
    //             rect.setFillColor(color);

    //             // Draw the square
    //             window.draw(rect);
    //         }
    //     }

    //     // Display the window
    //     window.display();
    // }

    // return 0;

}