#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "TileMap.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 740), "Minesweeper");
    
    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("tile.png"))
    {
        return -1;
    }

    const int lvlSize = 10;
    TileMap tile_map(lvlSize, tileTexture);
    tile_map.load("tile.png", sf::Vector2u(64, 64), tile_map.getTiles(), lvlSize, lvlSize, sf::Vector2f(0.f, 100.f));
    bool leftMousePressed = false;
    
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
                tile_map.input(sf::Mouse::getPosition(window));
                cout << "LEFT MOUSE PRESSED" << endl;
                leftMousePressed = true;
            }
            else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && leftMousePressed)
            {
                leftMousePressed = false;
            }
        }

        //drawing stuff
        window.clear();
        window.draw(tile_map);
        window.display();
    }

    return 0;
}