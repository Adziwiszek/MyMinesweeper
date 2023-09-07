#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "TileMap.h"
#include "UI.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 740), "Minesweeper");

    bool leftMousePressed = false;
    bool rightMousePressed = false;
    cout << sf::Mouse::Left << endl;

    //setting up tile map
    const int lvlSize = 10;
    TileMap tile_map;
    tile_map.init(lvlSize, "tile.png", "Poppins-Regular.ttf", 0.15);
    if (!tile_map.load(sf::Vector2u(64, 64), tile_map.getTiles(), lvlSize, lvlSize, sf::Vector2f(0.f, 100.f)))
    {
        cout << "asdewq" << endl;
    }

    while (window.isOpen())
    {
        //handling input
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !leftMousePressed)
            {
                tile_map.input(sf::Mouse::getPosition(window), sf::Mouse::Left);
                cout << "LEFT MOUSE PRESSED" << endl;
                leftMousePressed = true;
            }
            else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && leftMousePressed)
            {
                leftMousePressed = false;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !rightMousePressed)
            {
                tile_map.input(sf::Mouse::getPosition(window), sf::Mouse::Right);
                cout << "RIGHT MOUSE PRESSED" << endl;
                rightMousePressed = true;
            }
            else if (!sf::Mouse::isButtonPressed(sf::Mouse::Right) && rightMousePressed)
            {
                rightMousePressed = false;
            }
        }

        //drawing stuff
        window.clear();
        window.draw(tile_map);
        tile_map.drawText(window);
        window.display();
    }

    return 0;
}