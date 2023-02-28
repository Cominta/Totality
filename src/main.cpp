#include "engine.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "Totality", sf::Style::Fullscreen);
    Engine eng(&window);
    eng.start();

    return 0;
}