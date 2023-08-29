#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "TileMap.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 740), "Minesweeper");

    bool leftMousePressed = false;
    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("tile.png"))
    {
        return -1;
    }

    //setting up tile map
    const int lvlSize = 10;
    TileMap tile_map(lvlSize, tileTexture);
    tile_map.load("tile.png", sf::Vector2u(64, 64), tile_map.getTiles(), lvlSize, lvlSize, sf::Vector2f(0.f, 100.f));
    tile_map.createBombs(10);
    tile_map.uncoverWholeMap();

    //sf::Font fot;
    //fot.loadFromFile("Poppins-Regular.ttf");
    //sf::Text t1;
    //t1.setString("asd");
    //t1.setFont(fot);
    //t1.setColor(sf::Color::Red);
    //t1.setPosition(sf::Vector2f(0, 0));
    //t1.setCharacterSize(24);
    //t1.setStyle(sf::Text::Bold | sf::Text::Underlined);


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
        //window.draw(t1);
        /*for (int i = 0; i < lvlSize; i++)
        {
            for (int j = 0; j < lvlSize; j++)
            {
                window.draw(tile_map.tiles[i][j].bombsAroundText);
            }
        }*/
        //tile_map.drawText(window);
        window.display();
    }

    return 0;
}