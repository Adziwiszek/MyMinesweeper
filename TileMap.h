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
    // textureStatus:
    // 0 - covered 
    // 1 - uncovered
    // 2 - bomb
    int textureStatus;
    unsigned int bombsAround;
};


class TileMap : public sf::Drawable, sf::Transformable
{
public:
    //TileMap();
    TileMap(unsigned int lvlSize, sf::Texture tTexture);
    
    //clears any already existing bombs and populates map with new ones
    void createBombs(int nBombs);

    //loads map and setups it
    bool load(const std::string& tileset, sf::Vector2u tileSize, vector <vector<Tile>> new_tiles,
        unsigned int width, unsigned int height, sf::Vector2f startingPos/* = sf::Vector2f(0.f, 0.f)*/);

    //dealing with user input
    void input(sf::Vector2i mousePos);

    vector <vector<Tile>> getTiles();

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    //vector<vector<int>> tiles;
    vector<vector<Tile>> tiles;
    vector<int> bombsPos;
    int numOfBombs;
    unsigned int lvlSize;
    sf::Texture tileTexture;
};

#endif