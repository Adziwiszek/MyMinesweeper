#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

//#include "TileMap.cpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

using namespace std;

class TileMap : public sf::Drawable, sf::Transformable
{
public:
    TileMap(unsigned int lvlSize, sf::Texture tTexture);

    //loads map and setups it
    bool load(const std::string& tileset, sf::Vector2u tileSize, vector <vector<int>> new_tiles,
        unsigned int width, unsigned int height, sf::Vector2f startingPos/* = sf::Vector2f(0.f, 0.f)*/);

    void input(sf::Vector2i mousePos);

    vector <vector<int>> getTiles();

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    vector<vector<int>> tiles;
    int numOfBombs;
    unsigned int lvlSize;
    sf::Texture tileTexture;
};

#endif