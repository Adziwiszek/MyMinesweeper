#include "TileMap.h" 
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

using namespace std;

//Tile class
Tile::Tile()
{
    isCovered = true;
    isBomb = false;
    textureStatus = 0;
    bombsAround = 0;
}

//public
TileMap::TileMap(unsigned int lvlSize, sf::Texture tTexture)
{
    Tile mT;
    vector<Tile> row(lvlSize, mT);
    tiles = vector<vector<Tile>>(lvlSize, row);

    numOfBombs = ceil(lvlSize * lvlSize * 0.2);
    this->lvlSize = lvlSize;
    tileTexture = tTexture;
}

void TileMap::createBombs(int nBombs)
{
    srand(time(NULL));
    bombsPos.clear();
    vector<int> possiblePlaces;
    for (int i = 0; i < lvlSize * lvlSize; i++) 
        possiblePlaces.push_back(i);

    while (numOfBombs > 0)
    {
        int x = rand() % possiblePlaces.size();
        //bombsPos.push_back(x);
        tiles[x % lvlSize][x / lvlSize].isBomb = true;
        possiblePlaces.erase(possiblePlaces.begin() + x);
        numOfBombs--;
    }
}

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, vector <vector<Tile>> new_tiles,
    unsigned int width, unsigned int height, sf::Vector2f startingPos/* = sf::Vector2f(0.f, 0.f)*/)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
        return false;

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(width * height * 6);

    // populate the vertex array, with two triangles per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = new_tiles[i][j].textureStatus;

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            // get a pointer to the triangles' vertices of the current tile
            sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

            // define the 6 corners of the two triangles
            triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y) + startingPos;
            triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y) + startingPos;
            triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y) + startingPos;
            triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y) + startingPos;
            triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y) + startingPos;
            triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y) + startingPos;

            // define the 6 matching texture coordinates
            triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
        }

    return true;
}

void TileMap::input(sf::Vector2i mousePos)
{
    auto mouseIsInTile = [](sf::Vector2i mPos, sf::Vector2f tPos, float len) -> bool
    {
            if (mPos.x > tPos.x &&
                mPos.x < tPos.x + len &&
                mPos.y > tPos.y &&
                mPos.y < tPos.y + len)
            {
                return true;
            }
            else return false;
    };

    for (unsigned int i = 0; i < lvlSize; ++i)
    {
        for (unsigned int j = 0; j < lvlSize; ++j)
        {
            if (mouseIsInTile(mousePos, sf::Vector2f(j*64, i*64 + 100), 64))
            {
                if (tiles[j][i].isBomb)
                {
                    tiles[j][i].textureStatus = 2;
                }
                else
                {
                    tiles[j][i].textureStatus = 1;
                }
            }
        }
    }
    load("tile.png", sf::Vector2u(64, 64), tiles, lvlSize, lvlSize, sf::Vector2f(0.f, 100.f));
        
}

vector <vector<Tile>> TileMap::getTiles()
{
    return tiles;
}

//private
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}