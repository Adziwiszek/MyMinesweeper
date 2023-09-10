#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

//#include "TileMap.cpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

using namespace std;

class Tile
{
public:
    Tile();
    bool isCovered;
    bool isBomb;
    bool isFlagged;
    // textureStatus:
    // 0 - covered 
    // 1 - uncovered
    // 2 - bomb
    // 3 - red flag
    int textureStatus;
    unsigned int bombsAround;
    sf::Text numOfBombsAroundText;
};


class TileMap : public sf::Drawable, sf::Transformable
{
public:
    enum gameStates
    {
        won = 1,
        stillPlaying = 0,
        lost = -1
    };

    //using gameStates for this
    gameStates stateOfGame;

    //initialization of the map
    //difficulty - % of map that will be a bomb (eg: 10x10 map with difficulty = 0.15 will have 15 bombs)
    void init(unsigned int lvlSize, const string& m_tileset, const string& m_font, float difficulty);
    
    //clears any already existing bombs and populates map with new ones
    void createBombs(int nBombs);

    //loads map and setups it
    bool load(sf::Vector2u tileSize, vector <vector<Tile>> new_tiles, unsigned int width, unsigned int height, sf::Vector2f startingPos);

    void uncoverWholeMap();

    void uncoverMapAfterClick(sf::Vector2f clickedTile);

    //dealing with user input
    void input(sf::Vector2i mousePos, sf::Mouse::Button buttonPressed);

    vector <vector<Tile>> getTiles();

    void drawText(sf::RenderWindow& window);

    vector<vector<Tile>> tiles;
    
private:
   

    //calculates number of bombs around each tile
    void setTilesNumbers();
    sf::Color getTileColor(int nBombs);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    
    vector<int> bombsPos;
    int numOfBombs;
    int uncoveredBombs;
    

    unsigned int lvlSize;
    sf::Texture tileset;
    sf::Font tileFont;
};

#endif